import json
from pathlib import Path
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from ghidra import java_major
from harness import inspect, manifest_insert, tier
from workflow import compare, hint

BASE = 0x00500000
IMPORTS = {0x0070c5ac: 'vprintf'}


def classify(code, known=()):
    return inspect(bytes.fromhex(code), BASE, IMPORTS, set(known), lambda a: 0x00700000 <= a < 0x01000000)


class InspectTests(unittest.TestCase):
    def test_thiscall_getter_is_a_leaf(self):
        info = classify('8b 41 04 c3')  # mov eax,[ecx+4]; ret
        self.assertTrue(info['thiscall_hint'])
        self.assertEqual(tier(info), 'leaf')

    def test_eax_input_is_a_custom_convention(self):
        info = classify('8b 00 c3')  # mov eax,[eax]; ret
        self.assertIn('register-input', info['flags'])
        self.assertEqual(tier(info), 'blocked')

    def test_self_xor_and_saved_push_are_not_inputs(self):
        info = classify('56 33 c0 5e c3')  # push esi; xor eax,eax; pop esi; ret
        self.assertEqual(info['register_inputs'], [])

    def test_fastcall_is_standard(self):
        info = classify('8b c1 03 c2 c3')  # mov eax,ecx; add eax,edx; ret
        self.assertTrue(info['fastcall_hint'])
        self.assertNotIn('register-input', info['flags'])

    def test_relocations_and_callees(self):
        # call [vprintf]; mov eax,[0x007a18f8]; call known (inside, skipped); call unknown; ret 8
        code = 'ff 15 ac c5 70 00 a1 f8 18 7a 00 e8 f1 ff ff ff e8 00 10 00 00 c2 08 00'
        info = classify(code, known=[BASE + 1])
        self.assertEqual(info['imports'], ['vprintf'])
        self.assertEqual(info['globals'], [0x007a18f8])
        self.assertEqual(info['stack_bytes'], 8)
        self.assertEqual(info['calls_unknown'], [BASE + 21 + 0x1000])

    def test_seh_and_truncated_bodies_are_blocked(self):
        self.assertIn('seh-or-eh', classify('64 a1 00 00 00 00 c3')['flags'])
        self.assertIn('boundary-unverified', classify('8b 41 04')['flags'])


class ManifestTests(unittest.TestCase):
    def test_insert_keeps_order_and_one_object_per_line(self):
        text = '[\n{"id":"sub_00401000","size":4},\n{"id":"sub_00403000","size":4}\n]\n'
        out = manifest_insert(text, '{"id":"sub_00402000","size":8}')
        lines = out.splitlines()
        self.assertEqual(lines[2], '{"id":"sub_00402000","size":8},')
        self.assertEqual(len(json.loads(out)), 3)
        with self.assertRaises(ValueError):
            manifest_insert(out, '{"id":"sub_00402000","size":8}')


class CheckerOutputTests(unittest.TestCase):
    def test_score_counts_differing_instructions(self):
        expected = bytes.fromhex('8b 41 04 c3')
        self.assertEqual(compare(expected, expected, BASE)[0], 0)
        score, rows = compare(expected, bytes.fromhex('8b 41 08 c3'), BASE)
        self.assertEqual(score, 1)
        self.assertEqual([mark for mark, *_ in rows], ['|', ' '])

    def test_hints_name_the_config_to_change(self):
        row = {'id': 'sub_00479e00'}
        self.assertIn('"symbol":"_sub_004d6210","source":null',
                      hint(row, 'Unresolved relocation to _sub_004d6210').replace(' ', ''))
        self.assertIn('set "symbol":"?f@A@@QAEXXZ"',
                      hint(row, "Expected only function _x at section offset zero; got [('?f@A@@QAEXXZ', 0, 1, 32, 2)]"))


class JavaTests(unittest.TestCase):
    def test_release_file_version(self):
        with tempfile.TemporaryDirectory() as home:
            (Path(home) / 'release').write_text('JAVA_VERSION="25.0.1"\n')
            self.assertEqual(java_major(home), 25)
        self.assertEqual(java_major('/nonexistent'), 0)


if __name__ == '__main__':
    unittest.main()
