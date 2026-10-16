import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from coff import function_bytes
from extract import carve
from workflow import compiler


class ExtractionTests(unittest.TestCase):
    def test_embedded_members_and_truncation(self):
        data = b'prefix'
        for name in (b'data1.cab', b'data1.hdr', b'data2.cab'):
            data += name + b'\0Disk1\\' + name + b'\x000.0.0.0\x004\0ISc('
        self.assertEqual(set(carve(data)), {'data1.cab', 'data1.hdr', 'data2.cab'})
        with self.assertRaises(ValueError): carve(data[:-1])
        with self.assertRaises(ValueError): carve(data + data)


class WorkflowTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.root = Path(self.tmp.name)
        for name in ('config', 'src'):
            shutil.copytree(ROOT / name, self.root / name)
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
        source.write_text('invalid C++')
        self.assertNotEqual(self.run_check('check-function','sub_00401000').returncode,0)
        self.assertFalse((self.root/'build/reconstruction/sub_00401000.obj').exists())

    def test_wrong_reference_and_range(self):
        data = bytearray(self.ref.read_bytes()); data[4096] ^= 1; self.ref.write_bytes(data)
        self.assertIn('hash mismatch', self.run_check('check-overlay').stderr)
        shutil.copyfile(ROOT / json.loads((ROOT/'config/reference.json').read_text())['executable'], self.ref)
        path=self.root/'config/functions/sub_00401000.json'
        row=json.loads(path.read_text()); row['address']='0x400000';path.write_text(json.dumps(row))
        self.assertNotEqual(self.run_check('check-function','sub_00401000').returncode,0)

    def test_no_false_full_reconstruction(self):
        result=self.run_check('check-integrated')
        self.assertNotEqual(result.returncode,0)
        self.assertIn('BLOCKED',result.stderr)

    @unittest.skipUnless((ROOT / 'tools/local/msvc-wine-image.txt').exists(), 'optional Docker MSVC toolchain not installed')
    def test_msvc_manifest_profile(self):
        (self.root / 'tools').mkdir()
        (self.root / 'tools/local').symlink_to(ROOT / 'tools/local', target_is_directory=True)
        source = self.root / 'src/reconstruction/sub_00401000.cpp'
        source.write_text((ROOT / 'tests/qualification/msvc71/getter.cpp').read_text())
        manifest = self.root / 'config/functions/sub_00401000.json'
        row = json.loads(manifest.read_text())
        row['compiler'] = 'msvc71'
        manifest.write_text(json.dumps(row))
        result = self.run_check('check-function', 'sub_00401000')
        self.assertEqual(result.returncode, 0, result.stderr)
        source.write_text(source.read_text().replace('return field_0c;', 'return field_0c + 1;'))
        self.assertNotEqual(self.run_check('check-function', 'sub_00401000').returncode, 0)

    def test_symbol_and_relocation_rejection(self):
        result=self.run_check('check-function','sub_00401000')
        self.assertEqual(result.returncode,0,result.stderr)
        data=bytearray((self.root/'build/reconstruction/sub_00401000.obj').read_bytes())
        with self.assertRaises(ValueError): function_bytes(data, 'wrong_symbol')
        struct.pack_into('<H',data,20+32,1)
        with self.assertRaisesRegex(ValueError,'relocations'):
            function_bytes(data,'?sub_00401000@Object00401000@@QAEIXZ')


if __name__ == '__main__': unittest.main()
