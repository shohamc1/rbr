"""Inventory local dependencies and audit function boundaries without changing them."""
import argparse
from collections import Counter
from datetime import datetime, timezone
import json
import os
from pathlib import Path
import re
import subprocess
import zipfile

import pefile

from analyze import save
from boundaries import discovery
from extract import digest
from ghidra import ghidra_env
from workflow import reference

ROOT = Path(__file__).resolve().parents[1]
BUILD = ROOT / 'build/census'


def imports(pe):
    result = []
    for attribute, kind in [('DIRECTORY_ENTRY_IMPORT', 'import'),
                            ('DIRECTORY_ENTRY_DELAY_IMPORT', 'delay_import')]:
        for item in getattr(pe, attribute, []):
            result.append({'dll': item.dll.decode(), 'kind': kind,
                           'symbols': len(item.imports)})
    return result


def closure(roots, dependencies):
    """Follow only explicit edges; string presence is not a load edge."""
    visited = set()
    pending = list(roots)
    while pending:
        name = pending.pop().lower()
        if name in visited:
            continue
        visited.add(name)
        pending.extend(dependencies.get(name, []))
    return visited


def module_scope(modules, cfg):
    local = {Path(m['path']).name.lower(): m for m in modules}
    dependencies = {name: [i['dll'].lower() for i in row['imports']]
                    for name, row in local.items()}
    for edge in cfg['dynamic_loads']:
        dependencies.setdefault(edge['from'].lower(), []).append(edge['to'].lower())
    game = closure([cfg['game_root']], dependencies)
    auxiliary = closure(cfg['auxiliary_roots'], dependencies)
    return {'game_local_modules': sorted(local[n]['path'] for n in game if n in local),
            'auxiliary_local_modules': sorted(local[n]['path'] for n in auxiliary - game if n in local),
            'local_modules_without_load_edge': sorted(row['path'] for n, row in local.items()
                                                    if n not in game | auxiliary),
            'external_platform_imports': sorted(n for n in game | auxiliary if n not in local),
            'dynamic_loads': cfg['dynamic_loads'],
            'missing_optional_modules': cfg['missing_optional_modules'],
            'limitations': 'These are static dependency paths, not a gameplay load trace. Presence does not prove use. Windows loader policy can select a system copy instead of a bundled DLL.'}


def inventory():
    cfg, exe, _ = reference()
    artifacts = {x['path']: x['sha256'] for x in json.loads((ROOT / 'config/artifacts.json').read_text())}
    paths = sorted({exe, *Path('original/install/Root').rglob('*.dll'),
                    *Path('original/install/Root').rglob('*.exe')})
    rows = []
    for path in paths:
        sha = digest(path)
        if artifacts.get(str(path)) != sha:
            raise ValueError('Artifact hash mismatch or missing pin: ' + str(path))
        pe = pefile.PE(str(path))
        sections = [{'name': s.Name.rstrip(b'\0').decode(),
                     'address': hex(pe.OPTIONAL_HEADER.ImageBase + s.VirtualAddress),
                     'size': s.Misc_VirtualSize, 'raw_size': s.SizeOfRawData}
                    for s in pe.sections if s.Characteristics & 0x20000000]
        versions = {}
        for group in getattr(pe, 'FileInfo', []):
            for item in group:
                for table in getattr(item, 'StringTable', []):
                    versions.update({k.decode(): v.decode(errors='replace') for k, v in table.entries.items()
                                     if k in (b'CompanyName', b'FileDescription', b'FileVersion', b'OriginalFilename')})
        strings = []
        # Both encodings matter: the game can call LoadLibraryA or LoadLibraryW.
        for encoding, pattern in [('ascii', rb'[\x20-\x7e]{4,}'),
                                  ('utf-16le', rb'(?:[\x20-\x7e]\x00){4,}')]:
            for match in re.finditer(pattern, path.read_bytes()):
                value = match.group().decode(encoding)
                if '.dll' in value.lower():
                    strings.append({'file_offset': hex(match.start()), 'encoding': encoding, 'value': value})
        exports = getattr(getattr(pe, 'DIRECTORY_ENTRY_EXPORT', None), 'symbols', [])
        rows.append({'path': str(path), 'sha256': sha, 'file_bytes': path.stat().st_size,
                     'version': versions, 'code_sections': sections,
                     'code_bytes': sum(s['size'] for s in sections), 'imports': imports(pe),
                     'export_count': len(exports),
                     'forwarders': [s.forwarder.decode() for s in exports if s.forwarder],
                     'dll_string_candidates': strings})
    save(ROOT / 'analysis/module-inventory.json', rows)
    return rows


def analyze(path):
    """Use a separate unseeded project. Never overwrite matching-work projects."""
    pins = {row['path']: row['sha256'] for row in inventory()}
    sha = digest(path)
    if pins.get(str(path)) != sha:
        raise ValueError('Module is not in the pinned base inventory: ' + str(path))
    settings = json.loads((ROOT / 'config/ghidra.json').read_text())
    base = BUILD / sha[:12]
    base.mkdir(parents=True, exist_ok=True)
    seed = base / 'empty-seeds.tsv'
    seed.write_text('')
    output = base / 'boundaries.json'
    output.unlink(missing_ok=True)
    (base / 'refinement.json').unlink(missing_ok=True)
    headless, env = ghidra_env()
    cmd = [headless,
           str(base), 'census', '-scriptPath', str(ROOT / 'tools/ghidra'), '-max-cpu', '4',
           '-import', str(path.resolve()), '-overwrite', '-processor', settings['language'],
           '-cspec', settings['compiler_spec'], '-analysisTimeoutPerFile', '900',
           '-preScript', 'SeedFunctions.java', str(seed), str(ROOT / 'config/ghidra.json'),
           '-postScript', 'AuditBoundaries.java', str(output), sha]
    log = base / 'analyze.log'
    print(f'Analyze {path}; log: {log}', flush=True)
    with log.open('w') as stream:
        result = subprocess.run(cmd, env=env, stdout=stream, stderr=subprocess.STDOUT)
    if result.returncode or not output.exists():
        raise RuntimeError('Analysis failed or incomplete: ' + str(log))
    validate(json.loads(output.read_text()), sha, settings)
    (base / 'baseline.json').write_bytes(output.read_bytes())
    for name in ['history.json', 'discovery.json', 'discovery-previous.json', 'pending.json']:
        (base / name).unlink(missing_ok=True)
    print(output, flush=True)


def refine(path, audit_only=False):
    sha = digest(path)
    base = BUILD / sha[:12]
    baseline = json.loads((base / 'boundaries.json').read_text())
    settings = json.loads((ROOT / 'config/ghidra.json').read_text())
    validate(baseline, sha, settings)
    decisions = json.loads((ROOT / 'config/census.json').read_text())
    discovered = discovery(pefile.PE(str(path)), baseline,
                           [int(d['address'], 16) for d in decisions['vtable_stops'] if Path(d['module']) == path])
    for item in decisions['returning_imports']:
        if digest(Path(item['evidence_module'])) != item['evidence_sha256']:
            raise ValueError('Boundary decision evidence hash mismatch')
    discovered['returning_imports'] = decisions['returning_imports']
    discovered['fragment_merges'] = [d for d in decisions['fragment_merges'] if Path(d['module']) == path]
    discovered['code_data_corrections'] = [d for d in decisions['code_data_corrections'] if Path(d['module']) == path]
    discovered['reviewed_entries'] = [d for d in decisions['reviewed_entries'] if Path(d['module']) == path]
    discovered['flow_repairs'] = [d for d in decisions.get('flow_repairs', []) if Path(d['module']) == path]
    discovered['jump_overrides'] = [d for d in decisions.get('jump_overrides', []) if Path(d['module']) == path]
    previous = base / 'discovery.json'
    if previous.exists():
        # Preserve the original discovery list when a second pass uses the new bodies.
        save(base / 'discovery-previous.json', json.loads(previous.read_text()))
    save(previous, discovered)
    output = base / 'boundaries.json'
    (base / 'boundaries-previous.json').write_bytes(output.read_bytes())
    output.unlink(missing_ok=True)
    receipt = base / 'refinement.json'
    if not audit_only:
        receipt.unlink(missing_ok=True)
    headless, env = ghidra_env()
    cmd = [headless,
           str(base), 'census', '-scriptPath', str(ROOT / 'tools/ghidra'), '-max-cpu', '4',
           '-process', path.name, '-noanalysis']
    if audit_only:
        cmd += ['-readOnly']
    else:
        cmd += ['-postScript', 'RefineBoundaries.java', str(base / 'discovery.json'), str(receipt)]
    cmd += ['-postScript', 'AuditBoundaries.java', str(output), sha]
    log = base / ('audit.log' if audit_only else 'refine.log')
    print(f'{"Audit" if audit_only else "Refine"} {path}; log: {log}', flush=True)
    with log.open('w') as stream:
        result = subprocess.run(cmd, env=env, stdout=stream, stderr=subprocess.STDOUT)
    if result.returncode or not output.exists() or (not audit_only and not receipt.exists()):
        raise RuntimeError('Refinement failed or incomplete: ' + str(log))
    updated = json.loads(output.read_text())
    validate(updated, sha, settings)
    if audit_only:
        print(output, flush=True)
        return
    if json.loads(receipt.read_text())['sha256'] != sha:
        raise ValueError('Refinement receipt hash mismatch')
    history_path = base / 'history.json'
    history = json.loads(history_path.read_text()) if history_path.exists() else []
    history.append({'before': len(baseline['functions']), 'after': len(updated['functions']),
                    'vtable_seeds': len(discovered['vtable_seeds']),
                    'padded_leaf_seeds': len(discovered['padded_leaf_seeds']),
                    'refinement': json.loads(receipt.read_text())})
    save(history_path, history)
    print(output, flush=True)



def decompile(path):
    """Export every candidate in a saved census project; preserve explicit failures."""
    sha = digest(path)
    base = BUILD / sha[:12]
    audit = json.loads((base / 'boundaries.json').read_text())
    settings = json.loads((ROOT / 'config/ghidra.json').read_text())
    validate(audit, sha, settings)
    out = base / 'decompiled'
    out.mkdir(exist_ok=True)
    receipt = out / 'complete.json'
    receipt.unlink(missing_ok=True)
    headless, env = ghidra_env()
    cmd = [headless,
           str(base), 'census', '-scriptPath', str(ROOT / 'tools/ghidra'), '-max-cpu', '4',
           '-process', path.name, '-noanalysis', '-readOnly', '-postScript', 'DecompileCensus.java', str(out), sha]
    log = base / 'decompile.log'
    print(f'Decompile {path}; log: {log}', flush=True)
    with log.open('w') as stream:
        result = subprocess.run(cmd, env=env, stdout=stream, stderr=subprocess.STDOUT)
    if result.returncode or not receipt.exists():
        raise RuntimeError('Decompilation export incomplete: ' + str(log))
    metadata = json.loads(receipt.read_text())
    validate_decompilation(audit, metadata)
    validate_pseudocode(audit, metadata, out / 'pseudocode.jsonl')
    print(path, metadata['completed'], 'pseudocode exports;', len(metadata['failed_entries']), 'failures', flush=True)


def validate_decompilation(audit, metadata):
    if (metadata['sha256'] != audit['sha256'] or metadata['ghidra_version'] != audit['ghidra_version'] or
            metadata['entries'] != [f['address'] for f in audit['functions']]):
        raise ValueError('Decompilation export does not match current census')
    if (len(set(metadata['failed_entries'])) != len(metadata['failed_entries']) or
            not set(metadata['failed_entries']) <= set(metadata['entries'])):
        raise ValueError('Invalid decompilation failure list')
    if metadata['completed'] + len(metadata['failed_entries']) != len(metadata['entries']):
        raise ValueError('Decompilation export count does not balance')


def validate_pseudocode(audit, metadata, path):
    failures = []
    with path.open() as stream:
        for function in audit['functions']:
            line = stream.readline()
            if not line:
                raise ValueError('Truncated pseudocode export')
            row = json.loads(line)
            if row['address'] != function['address'] or row['size'] != function['size']:
                raise ValueError('Pseudocode body differs from current census')
            if row['completed']:
                if not isinstance(row.get('pseudocode'), str) or not row['pseudocode'].strip():
                    raise ValueError('Missing completed pseudocode')
            else:
                failures.append(row['address'])
        if stream.read().strip() or failures != metadata['failed_entries']:
            raise ValueError('Pseudocode export results do not balance')


def validate(report, sha, settings):
    if report['sha256'] != sha or report['ghidra_version'] != settings['version']:
        raise ValueError('Boundary export hash or version mismatch')
    for name, expected in settings['analyzers'].items():
        if report['analysis_options'].get(name) != expected:
            raise ValueError('Analyzer setting mismatch: ' + name)


def validate_decisions(report, cfg, path):
    entries = {f['address']: f for f in report['functions']}
    for decision in cfg['returning_imports']:
        for external in report.get('external_functions', []):
            if (external.get('original_import') == decision['symbol'] and
                    external['name'].split('::', 1)[0].lower() == decision['library'].lower() and
                    external['no_return']):
                raise ValueError('Reviewed returning import still marked no-return')
    for decision in cfg['fragment_merges']:
        if Path(decision['module']) == path and decision['fragment'] in entries:
            raise ValueError('Reviewed false function split remains')
    for decision in cfg['code_data_corrections']:
        if Path(decision['module']) == path and not entries.get(decision['address'], {}).get('entry_instruction'):
            raise ValueError('Reviewed call target is not a function entry')
    for decision in cfg.get('flow_repairs', []):
        if Path(decision['module']) == path and any(
                i['from'] == decision['from'] and i.get('to') == decision['target'] and
                i['kind'] in ('flow_to_noninstruction', 'fallthrough_outside_body', 'jump_to_unowned')
                for i in report.get('issues', [])):
            raise ValueError('Reviewed flow repair remains unresolved')
    for decision in cfg.get('reviewed_entries', []):
        if Path(decision['module']) == path and not entries.get(decision['address'], {}).get('entry_instruction'):
            raise ValueError('Reviewed entry is not a function entry')


def summarize(report):
    functions = report['functions']
    gaps = report['undefined_ranges']
    owned = report['function_body_union_bytes']
    unowned = sum(r['size'] for r in report['unowned_instruction_ranges'])
    data = report['data_outside_bodies_bytes']
    undefined = sum(r['size'] for r in gaps)
    if owned + unowned + data + undefined != report['executable_bytes']:
        raise ValueError('Code-section coverage does not balance')
    return {
        'function_candidates': len(functions),
        'thunks': sum(f['thunk'] for f in functions),
        'exception_funclet_name_candidates': sum(f.get('name', '').startswith(('Catch@', 'Unwind@')) for f in functions),
        'noncontiguous_bodies': sum(len(f['ranges']) > 1 for f in functions),
        'overlapping_body_bytes': sum(f['overlap_bytes'] for f in functions),
        'entries_without_instruction': [f['address'] for f in functions if not f['entry_instruction']],
        'bodies_outside_executable_bytes': sum(f['outside_executable_bytes'] for f in functions),
        'entries_without_call_jump_or_data_reference': sum(
            not (f['incoming_calls'] or f['incoming_jumps'] or f['incoming_data']) for f in functions),
        'executable_bytes': report['executable_bytes'],
        'function_body_union_bytes': owned,
        'unowned_instruction_bytes': unowned,
        'defined_data_outside_bodies_bytes': data,
        'undefined_bytes_by_kind': dict(Counter({kind: sum(r['size'] for r in gaps if r['kind'] == kind)
                                               for kind in sorted({r['kind'] for r in gaps})})),
        'issues_by_kind': dict(sorted(Counter(i['kind'] for i in report['issues']).items())),
        'computed_call_sites': len(report.get('computed_call_sites', [])),
        'computed_calls_without_flow_targets': sum(not c['resolved_flow_targets'] for c in report.get('computed_call_sites', [])),
        'failed_loader_decompilations': [r['address'] for r in report['loader_functions'] if not r['completed']],
        'exact_count_proved': False,
    }


def report():
    for name in ['function-census.json', 'module-scope.json']:
        (ROOT / 'analysis' / name).unlink(missing_ok=True)
    modules = inventory()
    settings = json.loads((ROOT / 'config/ghidra.json').read_text())
    cfg = json.loads((ROOT / 'config/census.json').read_text())
    rows = []
    for module in modules:
        path = BUILD / module['sha256'][:12] / 'boundaries.json'
        if not path.exists():
            if (path.parent / 'baseline.json').exists():
                raise ValueError('Incomplete census export: ' + str(path))
            continue
        evidence = json.loads(path.read_text())
        validate(evidence, module['sha256'], settings)
        validate_decisions(evidence, cfg, Path(module['path']))
        row = {'path': module['path'], 'sha256': module['sha256'],
               'evidence': str(path.relative_to(ROOT)), 'evidence_sha256': digest(path), **summarize(evidence)}
        row['pe_virtual_code_bytes'] = module['code_bytes']
        row['raw_section_alignment_bytes'] = sum(s['raw_size'] for s in module['code_sections']) - module['code_bytes']
        row['entries_outside_declared_code_sections'] = [f['address'] for f in evidence['functions'] if not any(
            int(s['address'], 16) <= int(f['address'], 16) < int(s['address'], 16) + s['size']
            for s in module['code_sections'])]
        history = path.parent / 'history.json'
        if history.exists():
            row['refinement_history'] = json.loads(history.read_text())
        baseline = path.parent / 'baseline.json'
        if baseline.exists():
            row['baseline_function_candidates'] = len(json.loads(baseline.read_text())['functions'])
        d = discovery(pefile.PE(module['path']), evidence,
                      [int(s['address'], 16) for s in cfg['vtable_stops'] if s['module'] == module['path']])
        save(path.parent / 'pending.json', d)
        row['rtti_vtables'] = len(d['vtables'])
        row['rtti_distinct_targets'] = d['distinct_vtable_targets']
        row['rtti_targets_in_body_interiors'] = d['vtable_targets_in_existing_body_interiors']
        row['pending_rtti_entries'] = len(d['vtable_seeds'])
        row['pending_padded_leaves'] = len(d['padded_leaf_seeds'])
        row['pending_bounded_cfg_entries'] = len(d['bounded_cfg_seeds'])
        receipt = path.parent / 'decompiled/complete.json'
        row['pseudocode_export'] = {'status': 'not_exported'}
        if receipt.exists():
            metadata = json.loads(receipt.read_text())
            try:
                validate_decompilation(evidence, metadata)
                validate_pseudocode(evidence, metadata, receipt.parent / 'pseudocode.jsonl')
                row['pseudocode_export'] = {'status': 'current_entries_and_sizes', 'completed': metadata['completed'],
                                            'failed_entries': metadata['failed_entries'],
                                            'path': str(receipt.parent.relative_to(ROOT)),
                                            'pseudocode_sha256': digest(receipt.parent / 'pseudocode.jsonl'),
                                            'limitations': 'Generated pseudocode; not reviewed C/C++ or matching source'}
            except ValueError:
                row['pseudocode_export'] = {'status': 'stale_or_invalid_export'}
        row['other_padding_entry_candidates'] = len(d['padding_entry_candidates'])
        row['return_entry_candidates'] = len(d['return_entry_candidates'])
        rows.append(row)
        print(module['path'], row['function_candidates'], 'candidates;', row['issues_by_kind'])
    census = {
        'generated_at_utc': datetime.now(timezone.utc).isoformat(timespec='seconds'),
        'method': 'Unseeded Ghidra baseline; bounded RTTI-backed virtual tables, padded straight-line leaves, bounded branch graphs, reviewed callbacks, return contracts, code/data corrections, body fixups, and repeated analysis. No candidate is a source match.',
        'limitations': 'Static analysis cannot prove a complete count with unresolved indirect flow, data boundaries, or runtime-selected modules.',
        'tool_sha256': {name: digest(ROOT / name) for name in
                        ['tools/census.py', 'tools/boundaries.py', 'tools/ghidra/AuditBoundaries.java',
                         'tools/ghidra/RefineBoundaries.java', 'tools/ghidra/DecompileCensus.java', 'config/census.json', 'config/ghidra.json', 'config/reference.json']},
        'modules': rows}
    scope = module_scope(modules, cfg)
    data = []
    artifacts = {a['path']: a['sha256'] for a in json.loads((ROOT / 'config/artifacts.json').read_text())}
    for item in cfg['data_named_dll']:
        if digest(ROOT / item['archive']) != artifacts[item['archive']]:
            raise ValueError('Archive evidence hash mismatch')
        with zipfile.ZipFile(ROOT / item['archive']) as archive:
            payload = archive.read(item['member'])
        if len(payload) != item['size'] or payload[:2] == b'MZ':
            raise ValueError('Changed data-named-DLL evidence')
        data.append(item)
    scope['data_named_dll'] = data
    scope['census_configuration_sha256'] = digest(ROOT / 'config/census.json')
    for group in ['game_local_modules', 'auxiliary_local_modules']:
        paths = set(scope[group])
        scope[group + '_code_bytes'] = sum(m['code_bytes'] for m in modules if m['path'] in paths)
        scope[group + '_function_candidates'] = sum(r['function_candidates'] for r in rows if r['path'] in paths)
        scope[group + '_not_analyzed'] = sorted(paths - {r['path'] for r in rows})
    scope['reconstruction_queue'] = [
        {'path': path, 'scope': group, 'source_rebuild': 'not_complete',
         'boundary_status': 'candidate count; unresolved ranges remain'}
        for group in ['game_local_modules', 'auxiliary_local_modules'] for path in scope[group]]
    save(ROOT / 'analysis/module-scope.json', scope)
    save(ROOT / 'analysis/function-census.json', census)



def progress():
    """Show a validated saved census without requiring the ignored Ghidra projects."""
    path = ROOT / 'analysis/function-census.json'
    if not path.exists():
        raise ValueError('Census snapshot unavailable. Run make census after analysis.')
    snapshot = json.loads(path.read_text())
    for name, sha in snapshot['tool_sha256'].items():
        if digest(ROOT / name) != sha:
            raise ValueError('Census snapshot is stale: ' + name + '. Run make census.')
    modules = inventory()
    pinned = {m['path']: m for m in modules}
    rows = {r['path']: r for r in snapshot['modules']}
    missing_local = missing_exports = 0
    for name, row in rows.items():
        if (name not in pinned or row['sha256'] != pinned[name]['sha256'] or
                row['pe_virtual_code_bytes'] != pinned[name]['code_bytes']):
            raise ValueError('Census snapshot module mismatch: ' + name)
        evidence = ROOT / row['evidence']
        if evidence.exists():
            if digest(evidence) != row['evidence_sha256']:
                raise ValueError('Census evidence changed: ' + name + '. Run make census.')
        elif (evidence.parent / 'baseline.json').exists():
            raise ValueError('Census analysis incomplete locally: ' + name)
        else:
            missing_local += 1
        export = row['pseudocode_export']
        if export['status'] == 'current_entries_and_sizes':
            output = ROOT / export['path'] / 'pseudocode.jsonl'
            if not output.exists():
                missing_exports += 1
            elif digest(output) != export['pseudocode_sha256']:
                raise ValueError('Pseudocode export changed: ' + name + '. Run make census.')
    cfg = json.loads((ROOT / 'config/census.json').read_text())
    scope = module_scope(modules, cfg)
    core = set(json.loads((ROOT / 'config/reference.json').read_text())['reconstruct'])
    game = set(scope['game_local_modules'])
    groups = [('Core game', core), ('Runtime/crash support', game - core),
              ('Auxiliary programs', set(scope['auxiliary_local_modules']))]
    print('Boundary census snapshot: ' + snapshot['generated_at_utc'])
    print('Candidate entries include thunks and exception handlers. Exact counts remain unproved.')
    print(f"{'Module':26} {'Candidates':>12} {'Code bytes':>12} {'Pseudocode':>12}")
    for label, paths in groups:
        candidates = code_bytes = unowned = undefined = indirect = 0
        absent = []
        for name in sorted(paths):
            row = rows.get(name)
            size = pinned[name]['code_bytes']
            code_bytes += size
            if row is None:
                absent.append(name)
                count, exported = 'unknown', '-'
            else:
                candidates += row['function_candidates']
                unowned += row['unowned_instruction_bytes']
                undefined += row['undefined_bytes_by_kind'].get('unresolved', 0)
                indirect += row['issues_by_kind'].get('unresolved_indirect_jump', 0)
                export = row['pseudocode_export']
                count = f"{row['function_candidates']:,}"
                exported = f"{export['completed']:,}" if export['status'] == 'current_entries_and_sizes' else export['status']
            print(f"{Path(name).name:26} {count:>12} {size:>12,} {exported:>12}")
        count = f'at least {candidates:,}' if absent else f'{candidates:,}'
        print(f'{label}: {count} candidate entries; {code_bytes:,} code-section bytes')
        print(f'  Analyzed modules: {unowned:,} unowned instruction bytes; {undefined:,} unclassified bytes; {indirect:,} unresolved indirect jumps')
        if absent:
            print('  Not analyzed: ' + ', '.join(Path(n).name for n in absent))
    known_game = game & rows.keys()
    total = sum(rows[n]['function_candidates'] for n in known_game)
    count = f'at least {total:,}' if game - rows.keys() else f'{total:,}'
    print(f'Game plus runtime/crash: {count} candidate entries; '
          f"{sum(pinned[n]['code_bytes'] for n in game):,} code-section bytes")
    exports = [r['pseudocode_export'] for r in rows.values()
               if r['path'] in game | set(scope['auxiliary_local_modules']) and
               r['pseudocode_export']['status'] == 'current_entries_and_sizes']
    print(f"Pseudocode snapshot: {sum(e['completed'] for e in exports):,} completed; "
          f"{sum(len(e['failed_entries']) for e in exports):,} failed; {len(exports)} modules. No source-match credit.")
    if missing_local:
        print(f'Local Ghidra evidence absent for {missing_local} modules; showing saved counts.')
    if missing_exports:
        print(f'Local pseudocode absent for {missing_exports} modules; showing saved export counts.')
    print('No supported load edge: ' + ', '.join(Path(n).name for n in scope['local_modules_without_load_edge']))
    print('Missing optional DLLs: ' + ', '.join(m['name'] for m in scope['missing_optional_modules']))
    print('Refresh saved counters with make census after analysis or export changes.')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('command', choices=['inventory', 'analyze', 'refine', 'audit', 'decompile', 'report', 'progress'])
    parser.add_argument('modules', nargs='*', type=Path)
    args = parser.parse_args()
    if args.command in ('analyze', 'refine', 'audit', 'decompile'):
        cfg = json.loads((ROOT / 'config/census.json').read_text())
        modules = inventory()
        scope = module_scope(modules, cfg)
        default = scope['game_local_modules'] + scope['auxiliary_local_modules']
        if args.command == 'decompile':
            default = [p for p in default if p.lower().endswith('.dll')]
        default += [m['path'] for m in modules if Path(m['path']).name.lower() in
                    {n.lower() for n in cfg['review_modules']} and args.command != 'decompile']
        pinned_paths = {m['path'].lower(): Path(m['path']) for m in modules}
        for path in args.modules or [Path(p) for p in default]:
            if str(path).lower() not in pinned_paths:
                raise ValueError('Module is not in the pinned base inventory: ' + str(path))
            path = pinned_paths[str(path).lower()]
            if args.command == 'decompile':
                decompile(path)
            elif args.command == 'analyze':
                analyze(path)
            else:
                refine(path, audit_only=args.command == 'audit')
    elif args.command == 'progress':
        progress()
    elif args.command == 'inventory':
        print('Inventoried', len(inventory()), 'base-install PE modules')
    else:
        report()


if __name__ == '__main__':
    os.chdir(ROOT)
    main()
