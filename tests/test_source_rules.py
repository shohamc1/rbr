"""Source and manifest rules that the byte checker can't enforce.

The checker accepts an absolute address compiled as an immediate, because the
reference holds the same 4 bytes. Such source hides what it refers to and
breaks under any other layout, so addresses must be named symbols instead."""
import json
import re
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCES = sorted((ROOT / 'src/reconstruction').glob('*.[ch]*'))
# The reference image spans 0x00400000-0x016ecfff; integer constants this large
# in this range are code or data addresses, not flags or sizes.
IMAGE = range(0x00400000, 0x016ed000)
HEX = re.compile(r'\b0x([0-9a-fA-F]{6,8})u?\b')
COMMENT = re.compile(r'//[^\n]*|/\*.*?\*/', re.S)
STRING = re.compile(r'"(?:\\.|[^"\\])*"')


def code_without_comments(text):
    return COMMENT.sub('', STRING.sub('""', text))


class SourceRuleTests(unittest.TestCase):
    def test_sources_name_addresses_instead_of_hard_coding_them(self):
        hits = []
        for path in SOURCES:
            for number, line in enumerate(code_without_comments(path.read_text()).splitlines(), 1):
                if 'LAYOUT_' in line:
                    continue
                for match in HEX.finditer(line):
                    if int(match.group(1), 16) in IMAGE:
                        hits.append(f'{path.relative_to(ROOT)}:{number}: {line.strip()}')
        self.assertEqual(hits, [], 'Declare an extern and map it in config/globals.json, declare the '
                         'function and give it a manifest entry, or write the literal (1.0f, "rb"):\n'
                         + '\n'.join(hits))

    def test_manifest_omits_empty_optional_fields(self):
        empty = [line for line in (ROOT / 'config/functions.json').read_text().splitlines()
                 if re.search(r'"[a-z_]+":(\[\]|\{\}|"")', line)]
        self.assertEqual(empty, [], 'Remove empty optional fields:\n' + '\n'.join(empty))

    def test_globals_are_unique(self):
        rows = json.loads((ROOT / 'config/globals.json').read_text())
        symbols = [row['symbol'] for row in rows]
        addresses = [int(row['address'], 16) for row in rows]
        self.assertEqual(len(symbols), len(set(symbols)), 'Duplicate global symbol')
        self.assertEqual(len(addresses), len(set(addresses)), 'Duplicate global address')

    def test_tracked_evidence_does_not_cite_ignored_reports(self):
        paths = [ROOT / 'analysis/notes.md', *SOURCES]
        hits = [str(p.relative_to(ROOT)) for p in paths if 'build/harness/reports' in p.read_text()]
        self.assertEqual(hits, [], 'Move the evidence into analysis/notes.md instead of citing an '
                         'ignored report: ' + ', '.join(hits))


if __name__ == '__main__':
    unittest.main()
