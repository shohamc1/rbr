"""Build, compare with reccmp, and audit every annotated function byte for byte.

reccmp compares disassembly with symbolic operands. A function counts as a
match only when reccmp scores it 100% and its bytes equal the reference
exactly, apart from the relocated operand fields whose targets reccmp checked.
Functions marked `// NONMATCHING` are drafts; every other annotation must match.

usage: check.py            check everything and print progress
       check.py 0xADDRESS  print the reccmp diff for one function
"""
import csv
import json
import os
from pathlib import Path
import re
import subprocess
import sys

import pefile

import build

ROOT, BUILD = build.ROOT, build.BUILD
NONMATCHING_RE = re.compile(r'^// NONMATCHING\n// FUNCTION: RBR (0x[0-9a-f]+)$', re.M)
ENV = {**os.environ, 'PATH': f'{ROOT / "tools/bin"}{os.pathsep}{os.environ["PATH"]}'}


def reccmp(*args):
    if not (ROOT / 'reccmp-user.yml').exists():
        reference = (ROOT / 'original/nocd/richardburnsrally.exe').resolve()
        (ROOT / 'reccmp-user.yml').write_text(f'targets:\n  RBR:\n    path: {reference}\n')
    tool = Path(sys.executable).with_name('reccmp-reccmp')
    return subprocess.run([tool, '--target', 'RBR', '--no-color', *args], cwd=BUILD, env=ENV,
                          capture_output=True, text=True)


def sizes():
    with (ROOT / 'config/functions.csv').open(newline='') as stream:
        return {int(row['address'], 16): int(row['size']) for row in csv.DictReader(stream)}


def audit(row, reference, recompiled, recomp_address, size):
    """Return why the bytes differ, or None when they match exactly."""
    symbols, sections = build.read_coff(ROOT / row['object'])
    section, value, _ = symbols[row['symbol']]
    length, raw, relocs = sections[section - 1]
    if value != 0:
        return 'function does not start its section; compile with /Gy (implied by /O2)'
    if length != size:
        return f'size {length} bytes, reference {size} bytes'
    expected = reference.get_data(int(row['address'], 16) - reference.OPTIONAL_HEADER.ImageBase, size)
    actual = bytearray(recompiled.get_data(recomp_address - recompiled.OPTIONAL_HEADER.ImageBase, size))
    expected = bytearray(expected)
    for offset in relocs:  # relocated operands: reccmp compared their targets by name
        expected[offset:offset + 4] = actual[offset:offset + 4] = b'\0\0\0\0'
    if expected != actual:
        first = next(i for i in range(size) if expected[i] != actual[i])
        return f'bytes differ from offset 0x{first:x}'
    return None


def check():
    build.main()
    result = reccmp('--json', 'report.json', '--silent')
    if result.returncode or not (BUILD / 'report.json').exists():
        raise ValueError(result.stdout + result.stderr)
    report = {int(e['address'], 16): e for e in json.loads((BUILD / 'report.json').read_text())['data']}
    known = sizes()
    reference = pefile.PE(str(build.REFERENCE), fast_load=True)
    recompiled = pefile.PE(str(build.EXE), fast_load=True)
    drafts = {int(a, 16) for s in build.SOURCES.rglob('*.cpp') for a in NONMATCHING_RE.findall(s.read_text())}
    with (BUILD / 'functions.csv').open(newline='') as stream:
        rows = list(csv.DictReader(stream))
    failures, matched, matched_bytes, draft_scores = [], 0, 0, []
    for row in rows:
        address = int(row['address'], 16)
        entry = report.get(address)
        if address not in known:
            failures.append(f'{row["address"]} {row["source"]}: add its size to config/functions.csv')
            continue
        if entry is None or 'recomp' not in entry:
            failures.append(f'{row["address"]} {row["source"]}: reccmp did not pair it with {row["symbol"]}')
            continue
        why = audit(row, reference, recompiled, int(entry['recomp'], 16), known[address])
        exact = entry['matching'] == 1.0 and why is None
        if address in drafts:
            draft_scores.append((entry['matching'], row['address'], row['source']))
            if exact:
                print(f'{row["address"]} now matches: remove its // NONMATCHING line')
        elif exact:
            matched += 1
            matched_bytes += known[address]
        else:
            reason = why if entry['matching'] == 1.0 else f'reccmp {entry["matching"]:.1%}'
            failures.append(f'{row["address"]} {row["source"]}: {reason}; run make diff ADDR={row["address"]}')
    text = sum(s.Misc_VirtualSize for s in reference.sections if s.Characteristics & 0x20000000)
    print(f'{matched} functions match ({matched_bytes} bytes, {matched_bytes / text:.2%} of code); '
          f'{len(draft_scores)} nonmatching drafts')
    if failures:
        raise ValueError('FAIL\n' + '\n'.join(failures))


def diff(address):
    build.main()
    result = reccmp('--verbose', address)
    print('\n'.join(line for line in result.stdout.splitlines()
                    if not line.startswith(('[INFO]', '[ERROR] Failed to find function symbol'))))


if __name__ == '__main__':
    try:
        diff(sys.argv[1]) if len(sys.argv) > 1 else check()
    except (ValueError, OSError) as error:
        sys.exit(str(error))
