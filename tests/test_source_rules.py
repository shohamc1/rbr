"""Source and data rules that the byte check can't enforce.

The byte check masks relocated operands, and reccmp compares their targets by
name. An absolute address compiled as an immediate has no relocation, so it
passes when the reference holds the same 4 bytes. Such source hides what it
refers to and breaks under any other layout, so addresses must be symbols."""
import csv
import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCES = sorted((ROOT / 'src').rglob('*.[ch]*'))
# The reference image spans 0x00400000-0x016ecfff; integer constants this large
# in this range are code or data addresses, not flags or sizes.
IMAGE = range(0x00400000, 0x016ed000)
HEX = re.compile(r'\b0x([0-9a-fA-F]{6,8})u?\b')
COMMENT = re.compile(r'//[^\n]*|/\*.*?\*/', re.S)
STRING = re.compile(r'"(?:\\.|[^"\\])*"')
ANNOTATION = re.compile(r'^// (?:FUNCTION|STUB|LIBRARY|TEMPLATE|SYNTHETIC): RBR (0x[0-9a-f]+)', re.M)


def rows(name):
    with (ROOT / 'config' / name).open(newline='') as stream:
        return list(csv.DictReader(stream))


class SourceRuleTests(unittest.TestCase):
    def test_sources_name_addresses_instead_of_hard_coding_them(self):
        hits = []
        for path in SOURCES:
            code = COMMENT.sub('', STRING.sub('""', path.read_text()))
            for number, line in enumerate(code.splitlines(), 1):
                if 'LAYOUT_' in line:
                    continue
                for match in HEX.finditer(line):
                    if int(match.group(1), 16) in IMAGE:
                        hits.append(f'{path.relative_to(ROOT)}:{number}: {line.strip()}')
        self.assertEqual(hits, [], 'Declare an extern and add it to config/symbols.csv, or write '
                         'the literal (1.0f, "rb"):\n' + '\n'.join(hits))

    def test_every_function_has_a_reviewed_size(self):
        sizes = {int(row['address'], 16) for row in rows('functions.csv')}
        missing = [f'{path.relative_to(ROOT)}: {address}' for path in SOURCES
                   for address in ANNOTATION.findall(path.read_text()) if int(address, 16) not in sizes]
        self.assertEqual(missing, [], 'Add each function boundary to config/functions.csv:\n'
                         + '\n'.join(missing))

    def test_symbols_are_unique(self):
        symbols = [row['symbol'] for row in rows('symbols.csv')]
        self.assertEqual(len(symbols), len(set(symbols)), 'Duplicate symbol in config/symbols.csv')

    def test_csv_rows_are_sorted(self):
        for name in ('functions.csv', 'symbols.csv'):
            addresses = [int(row['address'], 16) for row in rows(name)]
            self.assertEqual(addresses, sorted(addresses), f'Sort config/{name} by address')


if __name__ == '__main__':
    unittest.main()
