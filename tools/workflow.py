"""Exact checks for the pinned RBR reference. No full linker exists yet."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

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
    rows = [read_json(p) for p in sorted(Path('config/functions').glob('*.json'))]
    seen = set()
    ranges = []
    for row in rows:
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
            if hashlib.sha256(body).hexdigest() != row['reference_sha256']:
                raise ValueError('Function range/hash differs from reviewed evidence: ' + row['id'])
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


def build(row):
    profile = row.get('compiler', 'clang')
    if profile == 'msvc71':
        from msvc import compile_object
        out = compile_object(row['source'], BUILD / (row['id'] + '.obj'), row.get('extra_flags', []))
        return function_bytes(out.read_bytes(), row['symbol'])
    if profile != 'clang':
        raise ValueError('Unknown compiler profile: ' + profile)
    path, flags = compiler()
    BUILD.mkdir(parents=True, exist_ok=True)
    out = BUILD / (row['id'] + '.obj')
    out.unlink(missing_ok=True)
    # Always compile the active source; a prior object cannot satisfy a check.
    subprocess.run([path, *flags, *row.get('extra_flags', []), '-c', row['source'], '-o', str(out)], check=True)
    return function_bytes(out.read_bytes(), row['symbol'])


def difference(expected, actual, address):
    lines = [f'Expected {len(expected)} bytes, got {len(actual)} bytes']
    for label, body in [('reference', expected), ('compiled', actual)]:
        lines.append(label + ': ' + body.hex(' '))
        for ins in Cs(CS_ARCH_X86, CS_MODE_32).disasm(body, address):
            lines.append(f'  {ins.address:08x}: {ins.mnemonic} {ins.op_str}')
    return '\n'.join(lines)


def check(row, pe):
    offset, expected = original_bytes(pe, row)
    actual = build(row)
    if actual != expected:
        raise ValueError(row['id'] + '\n' + difference(expected, actual, int(row['address'], 16)))
    print(f"MATCH active C/C++ {row['id']} {row['address']} {len(actual)} bytes")
    return offset, actual


def overlay(pe, rows):
    out = BUILD / 'reference-overlay.exe'
    out.unlink(missing_ok=True)
    image = bytearray(pe.__data__)
    total = 0
    for row in rows:
        if row['status'] != 'matching_cpp':
            continue
        offset, code = check(row, pe)
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
    if args.command == 'check-integrated':
        raise ValueError('BLOCKED: no complete source/link layout for the EXE or game DLLs. Use check-overlay for the bounded intermediate check.')
    if args.command == 'check-function':
        selected = [r for r in rows if r['id'] == args.id]
        if len(selected) != 1 or not selected[0].get('source'):
            raise ValueError('Unknown function or missing source: ' + str(args.id))
        check(selected[0], pe)
    elif args.command == 'check-overlay':
        overlay(pe, rows)
    elif args.command == 'build':
        for row in rows:
            if row.get('source'):
                build(row)
                print('BUILT ' + row['id'])
    elif args.command == 'progress':
        totals = {k: 0 for k in ['matching_cpp', 'nonmatching_cpp', 'assembly', 'original_fallback', 'unresolved']}
        for row in rows:
            if row['status'] not in totals:
                raise ValueError('Unknown progress category')
            if row['status'] == 'matching_cpp':
                check(row, pe)
            totals[row['status']] += row['size']
        text_size = sum(min(s.SizeOfRawData, s.Misc_VirtualSize) for s in pe.sections if s.Characteristics & 0x20000000)
        totals['unresolved'] += text_size - sum(totals.values())
        print(json.dumps({'scope': path.name + ' file-backed executable sections (padding included)', 'bytes': totals, 'total': text_size}, indent=2))
        print('All other module code and all shared data remain unresolved. Overlay fallback bytes are not recovered source.')


if __name__ == '__main__':
    try:
        main()
    except (ValueError, OSError, subprocess.CalledProcessError) as error:
        sys.exit(str(error))
