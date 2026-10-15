import hashlib
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from ghidra import address
import msvc


class AnalysisToolTests(unittest.TestCase):
    def test_address_format(self):
        for value in ('0x401000', '00401000', 'sub_00401000'):
            self.assertEqual(address(value), '00401000')
        for value in ('-1', '100000000', 'not-an-address'):
            with self.assertRaises(ValueError): address(value)

    def test_download_hash_rejects_changed_payload(self):
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / 'tool'
            with patch('msvc.urllib.request.urlretrieve', side_effect=lambda url, dest: dest.write_bytes(b'wrong')):
                with self.assertRaisesRegex(ValueError, 'hash mismatch'):
                    msvc.download('unused', path, hashlib.sha256(b'expected').hexdigest())
            self.assertFalse(path.exists())

    def test_failed_compile_cannot_reuse_old_object(self):
        import subprocess
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / 'stale.obj'
            path.write_bytes(b'old success')
            cfg = {'runner':'unused', 'compiler':'unused', 'flags':[]}
            with patch('msvc.verify', return_value=cfg), patch('msvc.subprocess.run', side_effect=subprocess.CalledProcessError(1, 'compile')):
                with self.assertRaises(subprocess.CalledProcessError):
                    msvc.compile_object('unused.cpp', path)
            self.assertFalse(path.exists())
