"""Find, claim, and scaffold matching work: doctor, candidates, new-function, lookup."""
import argparse
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86_const import X86_OP_IMM, X86_OP_MEM, X86_REG_INVALID, X86_REG_FS

ROOT = Path(__file__).resolve().parents[1]
OUT = Path('build/harness')
COOKIE = 0x007c9410
VOLATILE = {'eax', 'ecx', 'edx'}
SAVED = {'ebx', 'esi', 'edi', 'ebp'}
SPECIAL = {'cpuid', 'rdtsc', 'int', 'in', 'out', 'sysenter', 'hlt'}
BLOCKERS = {'seh-or-eh', 'switch-table', 'special-instruction', 'register-input'}


def parse_address(value):
    number = int(str(value).removeprefix('sub_').removeprefix('0x'), 16)
    if not 0 <= number <= 0xffffffff:
        raise ValueError('Expected a 32-bit hexadecimal address: ' + str(value))
    return number


def function_id(address):
    return f'sub_{address:08x}'


def inspect(code, address, imports, known, is_data):
    """Classify one body. imports maps IAT slot -> name, known is a set of
    manifest addresses, is_data(address) reports non-code image addresses.
    Register-input detection is a linear heuristic, not data-flow analysis."""
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    md.detail = True
    end = address + len(code)
    info = {'decoded': 0, 'last': None, 'calls_known': [], 'calls_unknown': [], 'imports': [],
            'globals': [], 'indirect_calls': 0, 'flags': set(), 'register_inputs': set(), 'stack_bytes': 0}
    written = set()
    for ins in md.disasm(code, address):
        info['decoded'] += ins.size
        info['last'] = ins.mnemonic
        if ins.mnemonic in SPECIAL:
            info['flags'].add('special-instruction')
        if ins.mnemonic.startswith('f'):
            info['flags'].add('x87')
        if 'xmm' in ins.op_str:
            info['flags'].add('sse')
        if ins.mnemonic == 'ret' and ins.operands:
            info['stack_bytes'] = ins.operands[0].imm
        reads, writes = ins.regs_access()
        names = {ins.reg_name(r) for r in reads}
        operands = ins.op_str.split(', ')
        if ins.mnemonic in ('xor', 'sub') and len(operands) == 2 and operands[0] == operands[1]:
            names.discard(operands[0])  # xor reg, reg zeroes; it does not read an input
        # Pushing a callee-saved register preserves it; reading it otherwise is an input.
        inputs = VOLATILE if ins.mnemonic == 'push' else VOLATILE | SAVED
        info['register_inputs'] |= (names & inputs) - written
        written |= {ins.reg_name(r) for r in writes}
        for op in ins.operands:
            if op.type == X86_OP_MEM:
                if op.mem.segment == X86_REG_FS:
                    info['flags'].add('seh-or-eh')
                disp = op.mem.disp & 0xffffffff
                if op.mem.base == X86_REG_INVALID and op.mem.index == X86_REG_INVALID:
                    if disp in imports:
                        info['imports'].append(imports[disp])
                    elif disp == COOKIE:
                        info['flags'].add('/GS')
                    elif is_data(disp):
                        info['globals'].append(disp)
                elif op.mem.base == X86_REG_INVALID and ins.mnemonic == 'jmp':
                    info['flags'].add('switch-table')
                if ins.mnemonic == 'call' and not (op.mem.base == X86_REG_INVALID and disp in imports):
                    info['indirect_calls'] += 1
            elif op.type == X86_OP_IMM:
                value = op.imm & 0xffffffff
                if ins.mnemonic in ('call', 'jmp') or ins.mnemonic.startswith('j'):
                    if address <= value < end:
                        continue
                    if ins.mnemonic.startswith('j') and ins.mnemonic != 'jmp':
                        info['flags'].add('cross-function-branch')
                    (info['calls_known'] if value in known else info['calls_unknown']).append(value)
                elif value == COOKIE:
                    info['flags'].add('/GS')
                elif is_data(value):
                    info['globals'].append(value)
        if ins.mnemonic == 'call' and ins.operands and ins.operands[0].type not in (X86_OP_IMM, X86_OP_MEM):
            info['indirect_calls'] += 1
    # ECX is thiscall or fastcall; EDX alone with it is fastcall. Others are custom conventions.
    if info['register_inputs'] - {'ecx', 'edx'}:
        info['flags'].add('register-input')
    info['fastcall_hint'] = 'edx' in info['register_inputs']
    info['thiscall_hint'] = 'ecx' in info['register_inputs'] and not info['fastcall_hint']
    info['complete'] = info['decoded'] == len(code) and info['last'] in ('ret', 'jmp')
    if not info['complete']:
        info['flags'].add('boundary-unverified')
    info['flags'] = sorted(info['flags'])
    info['register_inputs'] = sorted(info['register_inputs'])
    info['globals'] = sorted(set(info['globals']))
    info['calls_unknown'] = sorted(set(info['calls_unknown']))
    info['calls_known'] = sorted(set(info['calls_known']))
    return info


def difficulty(size, info):
    """Rank work: smaller and more self-contained is easier. Lower is easier."""
    return (size + 30 * len(info['calls_unknown']) + 6 * len(info['globals'])
            + 3 * len(set(info['imports'])) + 10 * info['indirect_calls'])


def tier(info):
    if set(info['flags']) & BLOCKERS or 'boundary-unverified' in info['flags']:
        return 'blocked'
    if info['calls_unknown']:
        return 'needs-callee-entries'
    if info['globals'] or info['imports'] or info['calls_known']:
        return 'relocations'
    return 'leaf'


# --- Reference access ----------------------------------------------------

def load():
    from workflow import entries, import_slots, reference
    cfg, path, pe = reference()
    rows = entries()
    slots = {address: name for name, address in import_slots(pe).items()}
    base = pe.OPTIONAL_HEADER.ImageBase
    data = []
    code = []
    for s in pe.sections:
        span = (base + s.VirtualAddress, base + s.VirtualAddress + max(s.Misc_VirtualSize, s.SizeOfRawData))
        (code if s.Characteristics & 0x20000000 else data).append(span)

    def is_data(value):
        return any(lo <= value < hi for lo, hi in data) and value not in slots

    return cfg, pe, rows, slots, is_data


def body(pe, address, size):
    return pe.get_data(address - pe.OPTIONAL_HEADER.ImageBase, size)


def census_functions(cfg):
    """Reviewed census boundaries, falling back to the Ghidra matching index."""
    from ghidra import shared_build
    sha = cfg['executable_sha256'][:12]
    census = shared_build(f'census/{sha}/boundaries.json')
    if census.exists():
        rows = json.loads(census.read_text())['functions']
        return 'census', [(int(r['address'], 16), r['size'], len(r['ranges']), r['thunk'], r.get('incoming_calls', 0)) for r in rows]
    index = shared_build(f'ghidra/{sha}/exports/functions.json')
    if index.exists():
        rows = json.loads(index.read_text())
        return 'ghidra', [(int(r['address'], 16), r['size'], len(r['body_ranges']), r['thunk'], 0) for r in rows]
    raise ValueError('No boundary evidence. Run make census-analyze census-refine (preferred) or make ghidra-analyze.')


def claimed():
    """Addresses named in local branch names, for example decomp/sub_0048cc00."""
    try:
        names = subprocess.run(['git', 'branch', '--all', '--format=%(refname:short)'],
                               capture_output=True, text=True, check=True).stdout
    except (OSError, subprocess.CalledProcessError):
        return set()
    return {int(m, 16) for m in re.findall(r'(?<![0-9a-f])([0-9a-f]{8})(?![0-9a-f])', names)}


# --- Commands ------------------------------------------------------------

def doctor(_args):
    results = []

    def record(state, name, detail, fix=''):
        results.append(state)
        print(f'{state:4} {name}: {detail}')
        if fix and state != 'PASS':
            print(f'     fix: {fix}')

    try:
        from workflow import reference
        cfg, path, _ = reference()
        record('PASS', 'reference', f'{path} matches the pinned SHA-256')
    except Exception as error:
        record('FAIL', 'reference', str(error), "make extract ISO='/absolute/path/to/disc.iso', or link original/ from a verified checkout")
        cfg = None
    try:
        from workflow import compiler
        clang, _ = compiler()
        record('PASS', 'clang', clang + ' matches config/toolchain.json')
    except Exception as error:
        version = ''
        tested = json.loads(Path('config/toolchain.json').read_text())['tested_path']
        if Path(tested).exists():
            version = subprocess.run([tested, '--version'], capture_output=True, text=True).stdout.splitlines()[0]
        record('FAIL', 'clang', f'{error} (installed: {version or "none"})',
               'install the pinned Clang build, or ask the integrator to requalify. Never edit the hash to pass a check. '
               'MSVC-profile functions still check without Clang; the full gate does not.')
    if shutil.which('docker') and subprocess.run(['docker', 'info'], capture_output=True).returncode == 0:
        record('PASS', 'docker', 'engine reachable')
        try:
            from msvc import verify
            image = verify()['docker_image']
            present = subprocess.run(['docker', 'image', 'inspect', image], capture_output=True).returncode == 0
            record('PASS' if present else 'FAIL', 'msvc', 'compiler hashes and Wine image ' + ('present' if present else 'missing'), 'make setup-msvc')
        except Exception as error:
            record('FAIL', 'msvc', str(error), 'make setup-msvc')
        stale = subprocess.run(['docker', 'ps', '-q', '--filter', 'name=rbr-msvc-'], capture_output=True, text=True).stdout.split()
        record('WARN' if stale else 'PASS', 'msvc containers', f'{len(stale)} running',
               'if no check is running: docker rm -f ' + ' '.join(stale))
    else:
        record('FAIL', 'docker', 'engine not reachable', 'start Docker (OrbStack), then rerun make doctor')
    try:
        from ghidra import ghidra_env, installed_version
        headless, env = ghidra_env()
        pinned = json.loads(Path('config/ghidra.json').read_text())['version']
        version = installed_version(headless)
        record('PASS' if Path(headless).exists() else 'FAIL', 'ghidra', f'{headless} with JAVA_HOME={env["JAVA_HOME"]}', 'brew install ghidra')
        record('PASS' if version == pinned else 'FAIL', 'ghidra version', f'installed {version}, pinned {pinned}',
               f'install Ghidra {pinned} (Homebrew upgrades replace it), or ask the integrator to requalify the pin')
    except Exception as error:
        record('FAIL', 'ghidra', str(error), 'brew install ghidra openjdk@25')
    if cfg:
        sha = cfg['executable_sha256'][:12]
        from ghidra import shared_build
        project = shared_build(f'ghidra/{sha}/project/rbr.gpr')
        record('PASS' if project.exists() else 'WARN', 'ghidra project', str(project),
               'make ghidra-analyze (once per checkout; a worktree can read the main checkout export packets)')
        census = shared_build(f'census/{sha}/boundaries.json')
        record('PASS' if census.exists() else 'WARN', 'census evidence', str(census),
               'make census-analyze census-refine; make candidates falls back to the Ghidra index')
    print('Doctor: ' + ('ready' if 'FAIL' not in results else 'fix the FAIL lines first'))
    return 1 if 'FAIL' in results else 0


def candidates(args):
    cfg, pe, rows, slots, is_data = load()
    known = {int(r['address'], 16) for r in rows}
    spans = sorted((int(r['address'], 16), int(r['address'], 16) + r['size']) for r in rows if r.get('source'))
    taken = claimed()
    source, functions = census_functions(cfg)
    unresolved = {int(r['address'], 16): r for r in rows if r.get('source') is None}
    found = []
    for address, size, ranges, thunk, incoming in functions:
        if thunk or ranges != 1 or size > args.max_size or address in taken:
            continue
        if address in known and address not in unresolved:
            continue
        if any(lo < address + size and address < hi for lo, hi in spans):
            continue
        try:
            info = inspect(body(pe, address, size), address, slots, known, is_data)
        except Exception:
            continue
        found.append({'id': function_id(address), 'size': size, 'tier': tier(info), 'score': difficulty(size, info),
                      'in_manifest': address in unresolved, 'incoming_calls': incoming, **info})
    found.sort(key=lambda r: (r['tier'] == 'blocked', not r['in_manifest'], r['score']))
    OUT.mkdir(parents=True, exist_ok=True)
    (OUT / 'candidates.json').write_text(json.dumps(found, indent=1) + '\n')
    shown = [r for r in found if args.all or r['tier'] != 'blocked']
    if args.tier:
        shown = [r for r in shown if r['tier'] == args.tier]
    print(f'Boundary source: {source}. Excluding manifest sources and claimed branches. Lower score is easier.')
    print(f'{"id":13} {"size":>5} {"score":>5} {"tier":21} {"manifest":8} notes')
    for r in shown[:args.limit]:
        notes = []
        if r['thiscall_hint']:
            notes.append('ecx input: thiscall?')
        if r['fastcall_hint']:
            notes.append('ecx/edx input: fastcall?')
        if r['stack_bytes']:
            notes.append(f'ret {r["stack_bytes"]}')
        if r['calls_unknown']:
            notes.append('calls ' + ','.join(f'{a:08x}' for a in r['calls_unknown'][:3]))
        if r['imports']:
            notes.append('imports ' + ','.join(sorted(set(r['imports'])))[:40])
        if r['globals']:
            notes.append(f'{len(r["globals"])} globals')
        notes += [f for f in r['flags'] if f not in ('x87', 'sse')] + [f for f in r['flags'] if f in ('x87', 'sse')]
        print(f'{r["id"]:13} {r["size"]:5} {r["score"]:5} {r["tier"]:21} {"yes" if r["in_manifest"] else "no":8} {"; ".join(notes)}')
    print(f'{len(shown)} shown of {len(found)}; full list: {OUT / "candidates.json"}')
    print('Next: make worktree FUNCTION=<id>, then make new-function FUNCTION=<id> inside it')
    return 0


STUBS = {
    'thiscall': ('// thiscall member: ECX is read before it is written.\n'
                 'class {cls} {{\npublic:\n    void {fid}();\n}};\n\n'
                 'void {cls}::{fid}()\n{{\n    // TODO: body\n}}\n', '?{fid}@{cls}@@QAEXXZ'),
    'stdcall': ('// __stdcall: RET {n} removes {words} stack words.\n'
                'extern "C" void __stdcall {fid}({params})\n{{\n    // TODO: body\n}}\n', '_{fid}@{n}'),
    'cdecl': ('extern "C" void __cdecl {fid}()\n{{\n    // TODO: body\n}}\n', '_{fid}'),
}


def manifest_insert(text, line):
    """Insert one compact manifest object, keeping address order and one object per line."""
    lines = text.rstrip().splitlines()
    if lines[0] != '[' or lines[-1] != ']':
        raise ValueError('config/functions.json must hold one object per line between [ and ]')
    body_lines = [l.rstrip(',') for l in lines[1:-1]]
    key = json.loads(line)['id']
    if any(json.loads(l)['id'] == key for l in body_lines):
        raise ValueError('Manifest already has ' + key)
    body_lines.append(line)
    body_lines.sort(key=lambda l: json.loads(l)['id'])
    return '[\n' + ',\n'.join(body_lines) + '\n]\n'


def new_function(args):
    cfg, pe, rows, slots, is_data = load()
    address = parse_address(args.function)
    fid = function_id(address)
    known = {int(r['address'], 16) for r in rows}
    existing = next((r for r in rows if r['id'] == fid), None)
    if existing and existing.get('source') is not None:
        raise ValueError(f'{fid} already has a source: {existing["source"]}')
    size = args.size or (existing['size'] if existing else None)
    if size is None:
        _, functions = census_functions(cfg)
        match = [f for f in functions if f[0] == address]
        if not match:
            raise ValueError(f'No boundary evidence starts at {address:08x}; pass SIZE=<bytes> after reviewing the body')
        if match[0][2] != 1:
            raise ValueError(f'{fid} has {match[0][2]} body ranges; review its boundary and pass SIZE=<bytes>')
        size = match[0][1]
    for r in rows:
        lo = int(r['address'], 16)
        if r['id'] != fid and lo < address + size and address < lo + r['size']:
            raise ValueError(f'{fid} overlaps manifest entry {r["id"]}')
    info = inspect(body(pe, address, size), address, slots, known, is_data)
    if 'boundary-unverified' in info['flags']:
        print(f'WARNING: {fid} does not decode to a final RET/JMP in {size} bytes. Review the boundary before writing source.')
    if info['fastcall_hint']:
        print('NOTE: ECX and EDX are inputs; this looks like __fastcall. Change the scaffold convention; the checker reports the new symbol.')
    kind = 'thiscall' if info['thiscall_hint'] else 'stdcall' if info['stack_bytes'] else 'cdecl'
    words = info['stack_bytes'] // 4
    template, symbol = STUBS[kind]
    cls = 'Object' + fid[4:]
    fields = {'fid': fid, 'cls': cls, 'n': info['stack_bytes'], 'words': words,
              'params': ', '.join(f'unsigned int arg{i}' for i in range(words)) or 'void'}
    source = Path('src/reconstruction') / (fid + '.cpp')
    if source.exists():
        raise ValueError(f'{source} exists; edit it instead')
    source.write_text(f'// {fid}: EXE 0x{address:08x}, {size} bytes. Draft scaffold from make new-function.\n'
                      f'// Replace the convention, names, and body once the evidence supports them.\n\n'
                      + template.format(**fields))
    entry = {'id': fid, 'size': size, 'symbol': symbol.format(**fields), 'compiler': 'msvc71', 'nonmatching': True}
    if info['stack_bytes'] and kind == 'thiscall':
        print(f'NOTE: thiscall with RET {info["stack_bytes"]}: add {words} parameters; the checker reports the new symbol.')
    if '/GS' in info['flags']:
        entry['extra_flags'] = ['/GS']
    manifest = Path('config/functions.json')
    text = manifest.read_text()
    if existing:
        lines = [l for l in text.splitlines() if f'"id":"{fid}"' not in l.replace(' ', '')]
        text = '\n'.join(lines) + '\n'
        text = text.replace(',\n]', '\n]')
    manifest.write_text(manifest_insert(text, json.dumps(entry, separators=(',', ':'))))
    print(f'Wrote {source} and the {fid} entry in config/functions.json ({kind}, nonmatching).')
    report(info, slots)
    try:
        subprocess.run([sys.executable, 'tools/ghidra.py', 'function', fid], check=True)
    except subprocess.CalledProcessError:
        print('Ghidra export failed; run make doctor. You can still read the main checkout packet if it exists.')
    print(f'Next: write the body, then make check-function FUNCTION={fid}')
    return 0


def report(info, slots):
    if info['register_inputs']:
        print('Register inputs (heuristic): ' + ', '.join(info['register_inputs']))
    for target in info['calls_known']:
        print(f'Calls manifest function sub_{target:08x}: declare it with the symbol from config/functions.json.')
    for target in info['calls_unknown']:
        print(f'Calls sub_{target:08x}, which is not in the manifest. After the first compile, the checker prints '
              f'the exact manifest line to add with "source":null.')
    for name in sorted(set(info['imports'])):
        print(f'Uses import {name}: declare it extern "C" __declspec(dllimport) (C++ names need real declarations).')
    for value in info['globals']:
        print(f'References data 0x{value:08x}: declare it extern and add {{"symbol":"<mangled>","address":"0x{value:08x}"}} to config/globals.json.')
    blocking = [f for f in info['flags'] if f in BLOCKERS]
    if blocking:
        print('Blockers the checker cannot verify yet: ' + ', '.join(blocking) + '. See AGENTS.md before spending compiles.')


def worktree(args):
    """Create ../rbr-<id> on branch decomp/<id>, sharing verified inputs and tools with this checkout."""
    from ghidra import main_checkout
    fid = function_id(parse_address(args.function))
    main = main_checkout()
    path = main.parent / f'{main.name}-{fid}'
    subprocess.run(['git', 'worktree', 'add', '-b', f'decomp/{fid}', str(path), 'HEAD'], check=True)
    # Verified inputs and the local compiler are shared; build/ and .venv/ stay per worktree.
    for name in ('original', 'tools/local'):
        if (main / name).exists():
            (path / name).symlink_to(main / name)
    subprocess.run(['python3', '-m', 'venv', '.venv'], cwd=path, check=True)
    subprocess.run(['.venv/bin/pip', 'install', '-q', '-r', 'requirements.txt'], cwd=path, check=True)
    print(f'Created {path} on decomp/{fid}. The branch name claims {fid} in make candidates.')
    print(f'Next: cd {path} && make doctor && make new-function FUNCTION={fid}')
    return 0


def lookup(args):
    cfg, pe, rows, slots, is_data = load()
    address = parse_address(args.address)
    from workflow import data_symbols, import_thunks
    hits = []
    if address in slots:
        hits.append(f'IAT slot for import {slots[address]} (C source: __imp_{slots[address]} via __declspec(dllimport))')
    thunks = {a: n for n, a in import_thunks(pe, {n: a for a, n in slots.items()}).items()}
    if address in thunks:
        hits.append(f'jmp thunk to import {thunks[address]}')
    for name, value in data_symbols().items():
        if value == address:
            hits.append(f'global {name} in config/globals.json')
    for r in rows:
        lo = int(r['address'], 16)
        if lo <= address < lo + r['size']:
            hits.append(f'manifest {r["id"]} +0x{address - lo:x} ({r.get("symbol")}, {"unresolved" if r.get("source") is None else r["source"]})')
    try:
        _, functions = census_functions(cfg)
        for lo, size, *_ in functions:
            if lo <= address < lo + size:
                hits.append(f'boundary candidate sub_{lo:08x} +0x{address - lo:x}, {size} bytes')
    except ValueError:
        pass
    base = pe.OPTIONAL_HEADER.ImageBase
    for s in pe.sections:
        lo = base + s.VirtualAddress
        if lo <= address < lo + max(s.Misc_VirtualSize, s.SizeOfRawData):
            hits.append(f'section {s.Name.rstrip(bytes(1)).decode()} ({"code" if s.Characteristics & 0x20000000 else "data"})')
            if is_data(address) and address - lo < s.SizeOfRawData:
                raw = pe.get_data(address - base, 48).split(b'\0')[0]
                if raw and all(32 <= c < 127 for c in raw):
                    hits.append(f'string "{raw.decode()}"')
    print(f'0x{address:08x}:')
    for hit in hits or ['outside the image']:
        print('  ' + hit)
    return 0


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest='command', required=True)
    sub.add_parser('doctor', help='check every tool the workflow needs').set_defaults(run=doctor)
    p = sub.add_parser('candidates', help='rank unclaimed functions by expected difficulty')
    p.add_argument('--limit', type=int, default=25)
    p.add_argument('--max-size', type=int, default=128)
    p.add_argument('--tier', choices=['leaf', 'relocations', 'needs-callee-entries', 'blocked'])
    p.add_argument('--all', action='store_true', help='include blocked candidates')
    p.set_defaults(run=candidates)
    p = sub.add_parser('new-function', help='scaffold a source file and a nonmatching manifest entry')
    p.add_argument('function')
    p.add_argument('--size', type=int)
    p.set_defaults(run=new_function)
    p = sub.add_parser('worktree', help='claim a function in its own git worktree')
    p.add_argument('function')
    p.set_defaults(run=worktree)
    p = sub.add_parser('lookup', help='explain what an address is')
    p.add_argument('address')
    p.set_defaults(run=lookup)
    args = parser.parse_args()
    return args.run(args)


if __name__ == '__main__':
    os.chdir(ROOT)
    sys.path.insert(0, str(ROOT / 'tools'))
    try:
        sys.exit(main())
    except (ValueError, OSError, subprocess.CalledProcessError) as error:
        sys.exit(str(error))
