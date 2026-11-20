"""Exact checks for the pinned RBR reference. No full linker exists yet."""
import argparse
import difflib
import hashlib
import json
import os
import re
from pathlib import Path
import shutil
import subprocess
import sys
import time

import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from coff import function_bytes
from extract import digest

BUILD = Path('build/reconstruction')


def read_json(path):
    return json.loads(Path(path).read_text())


def reference():
    cfg = read_json('config/reference.json')
    path = Path(cfg['executable'])
    if digest(path) != cfg['executable_sha256']:
        raise ValueError('Reference executable hash mismatch: ' + str(path))
    return cfg, path, pefile.PE(str(path))


def entries():
    rows = read_json('config/functions.json')
    if not isinstance(rows, list) or not rows:
        raise ValueError('Function manifest must be a non-empty list')
    seen = set()
    ranges = []
    for row in rows:
        row.setdefault('address', '0x' + row['id'][4:])
        if type(row.get('nonmatching', False)) is not bool:
            raise ValueError('nonmatching must be a boolean')
        row.setdefault('source', f"src/reconstruction/{row['id']}.cpp")
        row['status'] = 'nonmatching_cpp' if row.get('nonmatching', False) else 'matching_cpp'
        if row['source'] is None:
            row['status'] = 'unresolved'
        if row['id'] in seen or row['size'] <= 0:
            raise ValueError('Duplicate id or invalid size')
        seen.add(row['id'])
        start = int(row['address'], 16)
        ranges.append((start, start + row['size']))
    for left, right in zip(sorted(ranges), sorted(ranges)[1:]):
        if left[1] > right[0]:
            raise ValueError('Overlapping function ranges')
    return rows


def original_bytes(pe, row):
    rva = int(row['address'], 16) - pe.OPTIONAL_HEADER.ImageBase
    size = row['size']
    for section in pe.sections:
        delta = rva - section.VirtualAddress
        if 0 <= delta and delta + size <= min(section.SizeOfRawData, section.Misc_VirtualSize):
            if not section.Characteristics & 0x20000000:
                raise ValueError('Function is outside executable sections')
            offset = section.PointerToRawData + delta
            body = pe.__data__[offset:offset + size]
            return offset, body
    raise ValueError('Function range is not fully backed by executable file bytes')


def compiler():
    cfg = read_json('config/toolchain.json')
    path = os.environ.get('RBR_CLANG') or shutil.which('clang-23') or cfg['tested_path']
    if digest(path) != cfg['sha256']:
        raise ValueError('Compiler hash mismatch. Qualify this tool before changing config/toolchain.json')
    for name, expected in cfg.get('libraries', {}).items():
        library = Path(path).resolve().parent.parent / 'lib' / name
        if digest(library) != expected:
            raise ValueError('Compiler library hash mismatch: ' + str(library))
    return path, cfg['flags']


def import_slots(pe):
    slots = {}
    for key in ('DIRECTORY_ENTRY_IMPORT', 'DIRECTORY_ENTRY_DELAY_IMPORT'):
        for descriptor in getattr(pe, key, []) or []:
            for entry in descriptor.imports:
                if entry.name and entry.address:
                    slots[entry.name.decode()] = entry.address
    return slots


def import_thunks(pe, slots):
    targets = {address: name for name, address in slots.items()}
    thunks = {}
    for section in pe.sections:
        if not section.Characteristics & 0x20000000:
            continue
        data = section.get_data()
        base = pe.OPTIONAL_HEADER.ImageBase + section.VirtualAddress
        for index in range(len(data) - 5):
            if data[index] == 0xFF and data[index + 1] == 0x25:
                name = targets.get(int.from_bytes(data[index + 2:index + 6], 'little'))
                if name is not None:
                    thunks.setdefault(name, base + index)
    return thunks


def data_symbols():
    path = Path('config/globals.json')
    if not path.exists():
        return {}
    rows = json.loads(path.read_text())
    return {row['symbol']: int(row['address'], 16) for row in rows}


def relocation_resolver(pe, rows):
    slots = import_slots(pe)
    thunks = import_thunks(pe, slots)
    addresses = {row['symbol']: int(row['address'], 16) for row in rows}
    data = data_symbols()

    def candidates(name):
        without = name.split('@')[0]
        yield name
        yield without
        if without.startswith('_'):
            yield without[1:]

    def resolve(name, kind):
        if kind == 20:
            if name in addresses:
                return addresses[name]
            return next((thunks[c] for c in candidates(name) if c in thunks), None)
        if kind == 6:
            if name.startswith('__imp_'):
                base = name[len('__imp_'):]
                if base in slots:
                    return slots[base]
                return next((slots[c] for c in candidates(base) if c in slots), None)
            if name in data:
                return data[name]
        return None

    return resolve


def compile_source(row):
    profile = row.get('compiler', 'clang')
    if profile == 'msvc71':
        from msvc import compile_object
        return compile_object(row['source'], BUILD / (row['id'] + '.obj'), row.get('extra_flags', []))
    if profile != 'clang':
        raise ValueError('Unknown compiler profile: ' + profile)
    path, flags = compiler()
    BUILD.mkdir(parents=True, exist_ok=True)
    out = BUILD / (row['id'] + '.obj')
    out.unlink(missing_ok=True)
    # Always compile the active source; a prior object cannot satisfy a check.
    subprocess.run([path, *flags, *row.get('extra_flags', []), '-c', row['source'], '-o', str(out)], check=True)
    return out


def listing(body, address):
    return [(f'{ins.address:08x}', f'{ins.mnemonic} {ins.op_str}'.strip())
            for ins in Cs(CS_ARCH_X86, CS_MODE_32).disasm(body, address)]


def compare(expected, actual, address):
    """Return (score, aligned rows). Score counts differing instructions; 0 is a match."""
    left, right = listing(expected, address), listing(actual, address)
    rows, score = [], 0
    matcher = difflib.SequenceMatcher(a=[t for _, t in left], b=[t for _, t in right], autojunk=False)
    for op, a0, a1, b0, b1 in matcher.get_opcodes():
        if op != 'equal':
            score += max(a1 - a0, b1 - b0)
        for i in range(max(a1 - a0, b1 - b0)):
            l = left[a0 + i] if a0 + i < a1 else ('', '')
            r = right[b0 + i] if b0 + i < b1 else ('', '')
            rows.append((' ' if op == 'equal' else '|', l, r))
    if score == 0 and expected != actual:
        score = 1  # same text, different encoding
    return score, rows


def difference(expected, actual, address):
    score, rows = compare(expected, actual, address)
    lines = [f'Expected {len(expected)} bytes, got {len(actual)} bytes; {score} differing instructions',
             f'  {"reference":44} {"compiled"}']
    for mark, (la, lt), (ra, rt) in rows:
        lines.append(f'{mark} {la:8} {lt:35} {ra:8} {rt}')
    lines.append('reference: ' + expected.hex(' '))
    lines.append('compiled:  ' + actual.hex(' '))
    return '\n'.join(lines)


def hint(row, message):
    """Turn a checker error into the next action for the source author."""
    found = re.search(r'Unresolved relocation to (\S+)', message)
    if found:
        name = found.group(1)
        target = re.search(r'sub_([0-9a-f]{8})', name)
        if name.startswith('__imp_'):
            return (f'{name} is not an EXE import. Run make lookup ADDR=<IAT slot from the reference instruction> '
                    'for the real name; C imports need extern "C" __declspec(dllimport).')
        if target:
            return ('If this is a call, add this line to config/functions.json:\n'
                    f'  {{"id":"sub_{target.group(1)}","size":<bytes from make lookup ADDR=0x{target.group(1)}>,"symbol":"{name}","source":null}}\n'
                    f'If it is data, add {{"symbol":"{name}","address":"0x{target.group(1)}"}} to config/globals.json.')
        return (f'Map {name} to an address: add {{"symbol":"{name}","address":"0x<address>"}} to config/globals.json '
                'for data, or a "source":null entry to config/functions.json for a function.')
    found = re.search(r'Expected only function (\S+) at section offset zero; got (\[.*\])', message, re.S)
    if found:
        names = re.findall(r"\('([^']+)'", found.group(2))
        if len(names) == 1:
            return f'The object defines {names[0]}. If that is this function, set "symbol":"{names[0]}" in config/functions.json.'
        return (f'The object defines {len(names)} functions: {", ".join(names) or "none"}. The checker needs exactly one. '
                'Make helpers inline or declare-only, and avoid naming destructors (they emit ??_G COMDATs).')
    if 'Additional code/data' in message:
        return ('The object has its own data, such as a string literal, float constant, switch table, or vtable. '
                'If the reference uses an absolute address, declare the data extern and map it in config/globals.json.')
    if message.startswith(row['id']):
        return ('Edit the source and rerun. Lines marked | differ. After eight compiles without a lower score, '
                'stop: keep the draft nonmatching and record the difference in analysis/notes.md.')
    return ''


def record_attempt(row, score, size):
    """Log one check per distinct source text and report progress toward the eight-compile rule."""
    log = Path('build/harness/attempts') / (row['id'] + '.jsonl')
    log.parent.mkdir(parents=True, exist_ok=True)
    source = Path(row['source']).read_bytes()
    stamp = hashlib.sha256(source + json.dumps(row.get('extra_flags', [])).encode()).hexdigest()
    history = [json.loads(l) for l in log.read_text().splitlines()] if log.exists() else []
    if not history or history[-1]['source'] != stamp:
        history.append({'source': stamp, 'score': score, 'size': size, 'time': time.strftime('%Y-%m-%dT%H:%M:%S')})
        log.write_text(''.join(json.dumps(h) + '\n' for h in history))
    best = min(range(len(history)), key=lambda i: (history[i]['score'], i))
    stale = len(history) - 1 - best
    print(f"Attempt {len(history)} for {row['id']}: score {score} (best {history[best]['score']} at attempt {best + 1}); "
          f"{stale} compiles since the best.{' Stop: eight compiles without improvement.' if stale >= 8 else ''}")


def check(row, pe, resolve=None, track=False):
    offset, expected = original_bytes(pe, row)
    out = compile_source(row)
    try:
        actual = function_bytes(out.read_bytes(), row['symbol'], int(row['address'], 16), resolve)
    except ValueError as error:
        advice = hint(row, str(error))
        raise ValueError(str(error) + ('\nhint: ' + advice if advice else '')) from None
    score = 0 if actual == expected else compare(expected, actual, int(row['address'], 16))[0]
    if track:
        record_attempt(row, score, len(actual))
    if actual != expected:
        message = row['id'] + '\n' + difference(expected, actual, int(row['address'], 16))
        raise ValueError(message + '\nhint: ' + hint(row, message))
    print(f"MATCH active C/C++ {row['id']} {row['address']} {len(actual)} bytes")
    if row.get('nonmatching'):
        print('hint: remove "nonmatching": true from its config/functions.json entry, then run make test check-overlay progress')
    return offset, actual


def overlay(pe, rows, resolve=None):
    out = BUILD / 'reference-overlay.exe'
    out.unlink(missing_ok=True)
    image = bytearray(pe.__data__)
    total = 0
    for row in rows:
        if row['status'] != 'matching_cpp':
            continue
        offset, code = check(row, pe, resolve)
        image[offset:offset + len(code)] = code
        total += len(code)
    if total == 0:
        raise ValueError('No compiled replacements; refusing an empty overlay')
    out.write_bytes(image)
    if bytes(image) != bytes(pe.__data__):
        raise ValueError('Overlay differs from reference')
    print(f'PASS intermediate overlay: {total} compiled bytes, {len(image)-total} original fallback bytes; no normalization')
    print('This is NOT a full reconstruction or a linked executable build.')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('command', choices=['build', 'check-function', 'check-overlay', 'check-integrated', 'progress', 'verify-inputs'])
    parser.add_argument('id', nargs='?')
    args = parser.parse_args()
    if args.command == 'verify-inputs':
        for row in read_json('config/artifacts.json'):
            if digest(row['path']) != row['sha256']:
                raise ValueError('Artifact hash mismatch: ' + row['path'])
        print('PASS all recorded extracted artifacts')
        return
    cfg, path, pe = reference()
    rows = entries()
    for row in rows:
        original_bytes(pe, row)
    resolve = relocation_resolver(pe, rows)
    if args.command == 'check-integrated':
        raise ValueError('BLOCKED: no complete source/link layout for the EXE or game DLLs. Use check-overlay for the bounded intermediate check.')
    if args.command == 'check-function':
        selected = [r for r in rows if r['id'] == args.id]
        if len(selected) != 1 or not selected[0].get('source'):
            raise ValueError('Unknown function or missing source: ' + str(args.id))
        check(selected[0], pe, resolve, track=True)
    elif args.command == 'check-overlay':
        overlay(pe, rows, resolve)
    elif args.command == 'build':
        for row in rows:
            if row.get('source'):
                compile_source(row)
                print('BUILT ' + row['id'])
    elif args.command == 'progress':
        totals = {k: 0 for k in ['matching_cpp', 'nonmatching_cpp', 'assembly', 'original_fallback', 'unresolved']}
        function_counts = {k: 0 for k in totals}
        for row in rows:
            if row['status'] not in totals:
                raise ValueError('Unknown progress category')
            if row['status'] == 'matching_cpp':
                check(row, pe, resolve)
            totals[row['status']] += row['size']
            function_counts[row['status']] += 1
        text_size = sum(min(s.SizeOfRawData, s.Misc_VirtualSize) for s in pe.sections if s.Characteristics & 0x20000000)
        totals['unresolved'] += text_size - sum(totals.values())
        print(json.dumps({'scope': path.name + ' file-backed executable sections (padding included)', 'bytes': totals, 'manifest_functions': function_counts, 'total': text_size}, indent=2))
        print('All other module code and all shared data remain unresolved. Overlay fallback bytes are not recovered source.')


if __name__ == '__main__':
    try:
        main()
    except (ValueError, OSError, subprocess.CalledProcessError) as error:
        sys.exit(str(error))
