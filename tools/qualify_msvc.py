"""Compile representative original bodies with the optional MSVC candidate."""
import json
from pathlib import Path
from coff import function_bytes
from msvc import compile_object
from workflow import reference, difference


def main():
    cfg, path, pe = reference()
    probes = [
        ('getter', 0x401000, 4, '?sub_00401000@Object00401000@@QAEIXZ', 'tests/qualification/msvc71/getter.cpp', []),
        ('setter', 0x4068e0, 10, '?sub_004068e0@Object004068e0@@QAEXI@Z', 'tests/qualification/msvc71/setter.cpp', []),
        ('absolute', 0x406490, 7, '_sub_00406490', 'tests/qualification/msvc71/absolute.cpp', []),
        ('predicate', 0x401020, 11, '?present@Object@@QAEHXZ', 'tests/qualification/predicate.cpp', []),
        ('constructor', 0x406410, 26, '??0Pair@@QAE@MM@Z', 'tests/qualification/msvc71/constructor.cpp', ['/arch:SSE']),
    ]
    rows = []
    for name, address, size, symbol, source, flags in probes:
        obj = compile_object(source, f'build/compiler/{name}.obj', flags)
        actual = function_bytes(obj.read_bytes(), symbol)
        expected = pe.get_data(address - pe.OPTIONAL_HEADER.ImageBase, size)
        rows.append({'name': name, 'address': hex(address), 'size': size, 'source': source,
                     'flags': flags, 'match': actual == expected,
                     'difference': None if actual == expected else difference(expected, actual, address)})
        print(('MATCH' if actual == expected else 'MISMATCH') + ' ' + name)
    report = {'reference': str(path), 'reference_sha256': cfg['executable_sha256'],
              'compiler_profile': 'config/msvc71.json', 'probes': rows,
              'limits': 'Sample qualification only. No complete link, RTTI, exception handling, custom register ABI, or Windows runtime qualification.'}
    Path('build/compiler/qualification.json').write_text(json.dumps(report, indent=2) + '\n')
    if not all(row['match'] for row in rows): raise SystemExit(1)


if __name__ == '__main__': main()
