import struct
import sys
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from coff import function_bytes
import workflow


def coff_object(code, relocations=(), symbols=()):
    """Build a minimal i386 COFF object. symbols: (name, value, section, type, storage, aux)."""
    encoded = []
    strings = bytearray(4)
    for name, value, section, kind, storage, aux in symbols:
        raw = name.encode()
        if len(raw) <= 8:
            encoded.append((raw.ljust(8, b'\0'), value, section, kind, storage))
        else:
            encoded.append((b'\0\0\0\0' + struct.pack('<I', len(strings)), value, section, kind, storage))
            strings += raw + b'\0'
    struct.pack_into('<I', strings, 0, len(strings))
    symoff = 20 + 40 + len(code) + 10 * len(relocations)
    data = bytearray()
    data += struct.pack('<HHIIIHH', 0x14c, 1, 0, symoff, len(encoded), 0, 0)
    data += struct.pack('<8sIIIIIIHHI', b'.text', len(code), 0, len(code), len(data) + 40,
                        symoff - 10 * len(relocations), 0, len(relocations), 0, 0x20)
    data += code
    for offset, symidx, kind in relocations:
        data += struct.pack('<IIH', offset, symidx, kind)
    for name, value, section, kind, storage in encoded:
        data += struct.pack('<8sIhHBB', name, value, section, kind, storage, 0)
    data += strings
    return bytes(data)


DEFINED = (1, 0x20, 2)   # section, type, storage of a defined function
UNDEFINED = (0, 0x20, 2)


class CoffRelocationTests(unittest.TestCase):
    def test_relocation_free_function_unchanged(self):
        code = b'\xc3'
        data = coff_object(code, (), [('f', 0, *DEFINED, 0)])
        self.assertEqual(function_bytes(data, 'f'), code)

    def test_rel32_call_is_resolved(self):
        code = b'\xe8\x00\x00\x00\x00\xc3'
        data = coff_object(code, ((1, 1, 20),),
                           [('f', 0, *DEFINED, 0), ('g', 0, *UNDEFINED, 0)])
        actual = function_bytes(data, 'f', 0x401000, lambda name, kind: {('g', 20): 0x401100}[(name, kind)])
        self.assertEqual(actual, b'\xe8\xfb\x00\x00\x00\xc3')

    def test_dir32_import_is_resolved(self):
        code = b'\xff\x15\x00\x00\x00\x00\xc3'
        data = coff_object(code, ((2, 1, 6),),
                           [('f', 0, *DEFINED, 0), ('__imp__Foo@4', 0, *UNDEFINED, 0)])
        actual = function_bytes(data, 'f', 0x401000, lambda name, kind: 0x70c1f4)
        self.assertEqual(actual, b'\xff\x15\xf4\xc1\x70\x00\xc3')

    def test_relocation_addends_are_preserved(self):
        for kind, addend, expected in ((20, -4, 0xf7), (6, 4, 0x401104),
                                      (6, -4, 0x4010fc)):
            with self.subTest(kind=kind, addend=addend):
                code = b'\xe8' + struct.pack('<i', addend) + b'\xc3'
                data = coff_object(code, ((1, 1, kind),),
                                   [('f', 0, *DEFINED, 0), ('g', 0, *UNDEFINED, 0)])
                actual = function_bytes(data, 'f', 0x401000, lambda name, kind: 0x401100)
                self.assertEqual(struct.unpack_from('<I', actual, 1)[0], expected)

    def test_relocation_outside_function_fails(self):
        data = coff_object(b'\xc3', ((1, 1, 6),),
                           [('f', 0, *DEFINED, 0), ('g', 0, *UNDEFINED, 0)])
        with self.assertRaisesRegex(ValueError, 'outside function'):
            function_bytes(data, 'f', 0x401000, lambda name, kind: 0x401100)

    def test_defined_symbol_is_not_resolved_as_external(self):
        data = coff_object(b'\x00' * 4, ((0, 1, 6),),
                           [('f', 0, *DEFINED, 0), ('g', 1, 1, 0, 3, 0)])
        with self.assertRaisesRegex(ValueError, 'undefined external'):
            function_bytes(data, 'f', 0x401000, lambda name, kind: 0x401100)

    def test_long_symbol_name_is_decoded(self):
        name = '__imp__GetStartupInfoA@4'
        data = coff_object(b'\0' * 4, ((0, 1, 6),),
                           [('f', 0, *DEFINED, 0), (name, 0, *UNDEFINED, 0)])
        seen = []
        function_bytes(data, 'f', 0x401000, lambda name, kind: seen.append(name) or 0x70c1f4)
        self.assertEqual(seen, [name])

    def test_unresolved_relocation_fails(self):
        data = coff_object(b'\xe8\x00\x00\x00\x00', ((1, 1, 20),),
                           [('f', 0, *DEFINED, 0), ('unknown', 0, *UNDEFINED, 0)])
        with self.assertRaisesRegex(ValueError, 'Unresolved relocation'):
            function_bytes(data, 'f', 0x401000, lambda name, kind: None)

    def test_relocations_require_addresses(self):
        data = coff_object(b'\xe8\x00\x00\x00\x00', ((1, 1, 20),),
                           [('f', 0, *DEFINED, 0), ('g', 0, *UNDEFINED, 0)])
        with self.assertRaisesRegex(ValueError, 'known symbol addresses'):
            function_bytes(data, 'f')

    def test_unsupported_relocation_type_fails(self):
        data = coff_object(b'\x00\x00\x00\x00', ((0, 1, 7),),
                           [('f', 0, *DEFINED, 0), ('g', 0, *UNDEFINED, 0)])
        with self.assertRaisesRegex(ValueError, 'Unsupported relocation type 7'):
            function_bytes(data, 'f', 0x401000, lambda name, kind: 0x401000)

    def test_out_of_range_rel32_fails(self):
        data = coff_object(b'\xe8\x00\x00\x00\x00', ((1, 1, 20),),
                           [('f', 0, *DEFINED, 0), ('g', 0, *UNDEFINED, 0)])
        with self.assertRaisesRegex(ValueError, 'out of range'):
            function_bytes(data, 'f', 0x401000, lambda name, kind: 0xf01000000)


def fake_pe(slots, code):
    def section(characteristics, body):
        return SimpleNamespace(Characteristics=characteristics, get_data=lambda: body, VirtualAddress=0)
    return SimpleNamespace(
        OPTIONAL_HEADER=SimpleNamespace(ImageBase=0x400000),
        DIRECTORY_ENTRY_IMPORT=[SimpleNamespace(imports=[
            SimpleNamespace(name=name.encode(), address=address) for name, address in slots.items()])],
        DIRECTORY_ENTRY_DELAY_IMPORT=[],
        sections=[section(0x60000000, code)])


class ResolverTests(unittest.TestCase):
    def test_manifest_symbols_and_import_names_resolve(self):
        pe = fake_pe({'GetStartupInfoA': 0x70c1f4, '_controlfp': 0x70c200}, b'')
        rows = [{'symbol': '?sub_00471110@@YAHXZ', 'address': '0x00471110'}]
        with patch.object(workflow, 'data_symbols', return_value={'___security_cookie': 0x7c9410}):
            resolve = workflow.relocation_resolver(pe, rows)
        self.assertEqual(resolve('?sub_00471110@@YAHXZ', 20), 0x471110)
        self.assertEqual(resolve('_GetStartupInfoA@4', 20), None)  # no thunk in the fake section
        self.assertEqual(resolve('__imp__GetStartupInfoA@4', 6), 0x70c1f4)
        self.assertEqual(resolve('__imp__malloc', 6), None)
        self.assertEqual(resolve('?unknown@@YAXXZ', 20), None)
        self.assertEqual(resolve('___security_cookie', 6), 0x7c9410)
        self.assertEqual(resolve('_unknown_data', 6), None)

    def test_import_thunks_resolve_by_plain_name(self):
        pe = fake_pe({'GetStartupInfoA': 0x70c1f4}, b'\x90\xff\x25\xf4\xc1\x70\x00\xc3')
        resolve = workflow.relocation_resolver(pe, [])
        self.assertEqual(resolve('_GetStartupInfoA@4', 20), 0x400001)
        self.assertEqual(resolve('__imp__GetStartupInfoA@4', 6), 0x70c1f4)

    def test_import_thunk_at_section_end_resolves(self):
        pe = fake_pe({'GetStartupInfoA': 0x70c1f4}, b'\xff\x25\xf4\xc1\x70\x00')
        resolve = workflow.relocation_resolver(pe, [])
        self.assertEqual(resolve('_GetStartupInfoA@4', 20), 0x400000)


if __name__ == '__main__':
    unittest.main()
