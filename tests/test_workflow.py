import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from coff import function_bytes
from extract import carve
from workflow import compiler, entries, original_bytes, reference
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86_const import X86_OP_IMM


class ExtractionTests(unittest.TestCase):
    def test_embedded_members_and_truncation(self):
        data = b'prefix'
        for name in (b'data1.cab', b'data1.hdr', b'data2.cab'):
            data += name + b'\0Disk1\\' + name + b'\x000.0.0.0\x004\0ISc('
        self.assertEqual(set(carve(data)), {'data1.cab', 'data1.hdr', 'data2.cab'})
        with self.assertRaises(ValueError): carve(data[:-1])
        with self.assertRaises(ValueError): carve(data + data)


class ManifestTests(unittest.TestCase):
    def test_reviewed_startup_ranges_include_all_branches(self):
        reviewed = {'sub_00411bc0', 'sub_00471110', 'sub_004807c0',
                    'sub_0059ee80', 'sub_006f06e2'}
        _, _, pe = reference()
        decoder = Cs(CS_ARCH_X86, CS_MODE_32)
        decoder.detail = True
        for row in entries():
            if row['id'] not in reviewed:
                continue
            with self.subTest(function=row['id']):
                _, body = original_bytes(pe, row)
                start = int(row['address'], 16)
                instructions = list(decoder.disasm(body, start))
                self.assertEqual(sum(i.size for i in instructions), len(body))
                self.assertEqual(instructions[-1].mnemonic, 'ret')
                for instruction in instructions:
                    if instruction.mnemonic.startswith('j') and instruction.operands[0].type == X86_OP_IMM:
                        target = instruction.operands[0].imm
                        self.assertTrue(start <= target < start + len(body), hex(target))

    def test_manifest_validation(self):
        first = {'id': 'sub_00401000', 'address': '0x401000', 'size': 4}
        second = {'id': 'sub_00401010', 'address': '0x401010', 'size': 4}
        with patch('workflow.read_json', return_value=[first, second]) as read:
            self.assertEqual(entries(), [first, second])
            read.assert_called_once_with('config/functions.json')
        for invalid in ([], {}, [first, first],
                        [first, dict(second, address='0x401002')],
                        [dict(first, size=0)], [dict(first, nonmatching='false')]):
            with self.subTest(rows=invalid), patch('workflow.read_json', return_value=invalid):
                with self.assertRaises(ValueError): entries()

    def test_compact_entries_and_overrides(self):
        row = {'id': 'sub_00401000', 'size': 4}
        with patch('workflow.read_json', return_value=[dict(row)]):
            result = entries()[0]
        self.assertEqual(result['address'], '0x00401000')
        self.assertEqual(result['source'], 'src/reconstruction/sub_00401000.cpp')
        self.assertEqual(result['status'], 'matching_cpp')
        with patch('workflow.read_json', return_value=[dict(row, nonmatching=True)]):
            self.assertEqual(entries()[0]['status'], 'nonmatching_cpp')
        with patch('workflow.read_json', return_value=[dict(row, address='0x500000', source='other.cpp')]):
            result = entries()[0]
        self.assertEqual((result['address'], result['source']), ('0x500000', 'other.cpp'))
        with patch('workflow.read_json', return_value=[dict(row, source=None)]):
            self.assertEqual(entries()[0]['status'], 'unresolved')


class WorkflowTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        for name in ('config', 'src'):
            shutil.copytree(ROOT / name, self.root / name)
        # The overlay integration gate covers the full manifest; unit fixtures stay fixed.
        manifest = self.root / 'config/functions.json'
        rows = json.loads(manifest.read_text())
        fixture_ids = {'sub_00401000', 'sub_00406490', 'sub_004068e0'}
        manifest.write_text(json.dumps([row for row in rows if row['id'] in fixture_ids]))
        if (ROOT / 'tools/local/msvc-wine-image.txt').exists():
            (self.root / 'tools').mkdir()
            (self.root / 'tools/local').symlink_to(ROOT / 'tools/local', target_is_directory=True)
        cfg = json.loads((self.root / 'config/reference.json').read_text())
        self.ref = self.root / cfg['executable']
        self.ref.parent.mkdir(parents=True)
        shutil.copyfile(ROOT / cfg['executable'], self.ref)

    def run_check(self, *args):
        return subprocess.run([sys.executable, str(ROOT / 'tools/workflow.py'), *args], cwd=self.root, capture_output=True, text=True)

    def test_active_source_and_overlay(self):
        result = self.run_check('check-overlay')
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual((self.root/'build/reconstruction/reference-overlay.exe').read_bytes(), self.ref.read_bytes())
        source = self.root/'src/reconstruction/sub_00401000.cpp'
        source.write_text(source.read_text().replace('return field_0c;', 'return field_0c + 1;'))
        result = self.run_check('check-function', 'sub_00401000')
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('Expected 4 bytes', result.stderr)
        self.assertNotEqual(self.run_check('check-overlay').returncode, 0)
        self.assertNotEqual(self.run_check('progress').returncode, 0)
        source.write_text('invalid C++')
        self.assertNotEqual(self.run_check('check-function','sub_00401000').returncode,0)
        self.assertFalse((self.root/'build/reconstruction/sub_00401000.obj').exists())

    def test_wrong_reference_and_range(self):
        data = bytearray(self.ref.read_bytes()); data[4096] ^= 1; self.ref.write_bytes(data)
        self.assertIn('hash mismatch', self.run_check('check-overlay').stderr)
        shutil.copyfile(ROOT / json.loads((ROOT/'config/reference.json').read_text())['executable'], self.ref)
        path=self.root/'config/functions.json'
        rows=json.loads(path.read_text())
        next(row for row in rows if row['id']=='sub_00401000')['address']='0x400000'
        path.write_text(json.dumps(rows))
        self.assertNotEqual(self.run_check('check-function','sub_00401000').returncode,0)

    def test_build_keeps_drafts_with_extra_sections(self):
        source = self.root / 'src/reconstruction/sub_00401000.cpp'
        source.write_text('extern "C" int global; int global = 1; int f() { return global; }')
        manifest = self.root / 'config/functions.json'
        rows = json.loads(manifest.read_text())
        rows[0]['nonmatching'] = True
        manifest.write_text(json.dumps(rows))
        result = self.run_check('build')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertTrue((self.root / 'build/reconstruction/sub_00401000.obj').is_file())
        self.assertNotEqual(self.run_check('check-function', 'sub_00401000').returncode, 0)

    def test_no_false_full_reconstruction(self):
        result=self.run_check('check-integrated')
        self.assertNotEqual(result.returncode,0)
        self.assertIn('BLOCKED',result.stderr)

    def test_explicit_draft_is_excluded_from_matches(self):
        source = self.root / 'src/reconstruction/sub_00401000.cpp'
        source.write_text(source.read_text().replace('return field_0c;', 'return field_0c + 1;'))
        manifest = self.root / 'config/functions.json'
        rows = json.loads(manifest.read_text())
        next(row for row in rows if row['id'] == 'sub_00401000')['nonmatching'] = True
        manifest.write_text(json.dumps(rows))
        result = self.run_check('check-overlay')
        self.assertEqual(result.returncode, 0, result.stderr)
        result = self.run_check('progress')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('"nonmatching_cpp": 4', result.stdout)
        progress = json.loads(result.stdout[result.stdout.index('{'):result.stdout.rindex('}') + 1])
        self.assertEqual(progress['manifest_functions']['nonmatching_cpp'], 1)
        self.assertEqual(progress['manifest_functions']['matching_cpp'], len(rows) - 1)
        self.assertNotEqual(self.run_check('check-function', 'sub_00401000').returncode, 0)

    @unittest.skipUnless((ROOT / 'tools/local/msvc-wine-image.txt').exists(), 'optional Docker MSVC toolchain not installed')
    def test_msvc_manifest_profile(self):
        source = self.root / 'src/reconstruction/sub_00401000.cpp'
        source.write_text((ROOT / 'tests/qualification/msvc71/getter.cpp').read_text())
        manifest = self.root / 'config/functions.json'
        rows = json.loads(manifest.read_text())
        row = next(row for row in rows if row['id'] == 'sub_00401000')
        row['compiler'] = 'msvc71'
        manifest.write_text(json.dumps(rows))
        result = self.run_check('check-function', 'sub_00401000')
        self.assertEqual(result.returncode, 0, result.stderr)
        source.write_text(source.read_text().replace('return field_0c;', 'return field_0c + 1;'))
        self.assertNotEqual(self.run_check('check-function', 'sub_00401000').returncode, 0)

    def test_one_source_holds_several_functions(self):
        combined = self.root / 'src/reconstruction/combined.cpp'
        combined.write_text(''.join((self.root / f'src/reconstruction/{name}.cpp').read_text() + '\n'
                                    for name in ('sub_00401000', 'sub_004068e0', 'sub_00406490')))
        manifest = self.root / 'config/functions.json'
        rows = json.loads(manifest.read_text())
        for row in rows:
            row['source'] = 'src/reconstruction/combined.cpp'
        manifest.write_text(json.dumps(rows))
        result = self.run_check('check-overlay')
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual(result.stdout.count('MATCH'), 3)
        combined.write_text(combined.read_text().replace('return field_0c;', 'return field_0c + 1;'))
        result = self.run_check('check-function', 'sub_004068e0')
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertNotEqual(self.run_check('check-function', 'sub_00401000').returncode, 0)

    def test_symbol_and_relocation_rejection(self):
        result=self.run_check('check-function','sub_00401000')
        self.assertEqual(result.returncode,0,result.stderr)
        data=bytearray((self.root/'build/reconstruction/sub_00401000.obj').read_bytes())
        with self.assertRaises(ValueError): function_bytes(data, 'wrong_symbol')
        # Give the function's own section a relocation it can't resolve without addresses.
        count = struct.unpack_from('<H', data, 2)[0]
        text = next(20 + 40 * i for i in range(count)
                    if data[20 + 40 * i:20 + 40 * i + 5] == b'.text' and struct.unpack_from('<I', data, 20 + 40 * i + 16)[0])
        struct.pack_into('<H', data, text + 32, 1)
        with self.assertRaisesRegex(ValueError,'known symbol addresses'):
            function_bytes(data,'?sub_00401000@Object00401000@@QAEIXZ')


if __name__ == '__main__': unittest.main()
