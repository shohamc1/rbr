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


def coff_sections(sections, symbols):
    """Build an i386 COFF object with several sections.
    sections: (name, body, relocations, flags); symbols as in coff_object."""
    encoded, strings = [], bytearray(4)
    for name, value, section, kind, storage, aux in symbols:
        raw = name.encode()
        if len(raw) <= 8:
            encoded.append((raw.ljust(8, b'\0'), value, section, kind, storage))
        else:
            encoded.append((b'\0\0\0\0' + struct.pack('<I', len(strings)), value, section, kind, storage))
            strings += raw + b'\0'
    struct.pack_into('<I', strings, 0, len(strings))
    offset = 20 + 40 * len(sections)
    headers, payload = bytearray(), bytearray()
    for name, body, relocations, flags in sections:
        raw = offset + len(payload)
        payload += body
        relptr = offset + len(payload)
        for reloc in relocations:
            payload += struct.pack('<IIH', *reloc)
        headers += struct.pack('<8sIIIIIIHHI', name, len(body), 0, len(body), raw, relptr, 0, len(relocations), 0, flags)
    symoff = offset + len(payload)
    data = struct.pack('<HHIIIHH', 0x14c, len(sections), 0, symoff, len(encoded), 0, 0) + headers + payload
    for name, value, section, kind, storage in encoded:
        data += struct.pack('<8sIhHBB', name, value, section, kind, storage, 0)
    return bytes(data + strings)


CODE, DATA = 0x60001020, 0x40000040  # COMDAT code, initialized data


class MultiFunctionTests(unittest.TestCase):
    def test_each_function_is_read_from_its_own_section(self):
        data = coff_sections([(b'.text', b'\xc3', (), CODE), (b'.text', b'\x90\xc3', (), CODE)],
                             [('f', 0, 1, 0x20, 2, 0), ('g', 0, 2, 0x20, 2, 0)])
        self.assertEqual(function_bytes(data, 'f'), b'\xc3')
        self.assertEqual(function_bytes(data, 'g'), b'\x90\xc3')

    def test_call_to_function_in_same_object_resolves_by_name(self):
        data = coff_sections([(b'.text', b'\xe8\x00\x00\x00\x00\xc3', ((1, 1, 20),), CODE), (b'.text', b'\xc3', (), CODE)],
                             [('f', 0, 1, 0x20, 2, 0), ('g', 0, 2, 0x20, 2, 0)])
        actual = function_bytes(data, 'f', 0x401000, lambda name, kind: {'g': 0x401100}.get(name))
        self.assertEqual(actual, b'\xe8\xfb\x00\x00\x00\xc3')
        with self.assertRaisesRegex(ValueError, 'Unresolved relocation to g'):
            function_bytes(data, 'f', 0x401000, lambda name, kind: None)

    def test_object_local_data_is_rejected(self):
        data = coff_sections([(b'.text', b'\xa1\x00\x00\x00\x00\xc3', ((1, 1, 6),), CODE), (b'.data', b'\x01\0\0\0', (), DATA)],
                             [('f', 0, 1, 0x20, 2, 0), ('_global', 0, 2, 0, 2, 0)])
        with self.assertRaisesRegex(ValueError, 'object-local data'):
            function_bytes(data, 'f', 0x401000, lambda name, kind: 0x7a0000)

    def test_functions_sharing_a_section_are_rejected(self):
        data = coff_sections([(b'.text', b'\xc3\xc3', (), CODE)],
                             [('f', 0, 1, 0x20, 2, 0), ('g', 1, 1, 0x20, 2, 0)])
        with self.assertRaisesRegex(ValueError, 'shares its section'):
            function_bytes(data, 'f')


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


class LiteralTests(unittest.TestCase):
    FLD = b'\xd9\x05\x00\x00\x00\x00\xc3'  # fld dword ptr [literal]; ret

    def literal_object(self, blob=b'\x00\x00\x80\x3f'):
        return coff_sections([(b'.text', self.FLD, ((2, 1, 6),), CODE), (b'.rdata', blob, (), DATA)],
                             [('f', 0, 1, 0x20, 2, 0), ('__real@3f800000', 0, 2, 0, 2, 0)])

    @staticmethod
    def rdata_pe(body, address=0x776a3c):
        section = SimpleNamespace(Characteristics=0x40000040, VirtualAddress=address - 0x400000,
                                  SizeOfRawData=len(body), Misc_VirtualSize=len(body), get_data=lambda: body)
        return SimpleNamespace(OPTIONAL_HEADER=SimpleNamespace(ImageBase=0x400000), sections=[section])

    def setUp(self):
        workflow._literal_addresses.clear()

    def test_literal_takes_the_reference_address_when_bytes_agree(self):
        expected = b'\xd9\x05\x3c\x6a\x77\x00\xc3'
        literal = workflow.literal_resolver(self.rdata_pe(b'\x00\x00\x80\x3f'), expected)
        self.assertEqual(function_bytes(self.literal_object(), 'f', 0x401000, lambda n, k: None, literal), expected)

    def test_literal_with_other_reference_bytes_cannot_match(self):
        expected = b'\xd9\x05\x3c\x6a\x77\x00\xc3'
        literal = workflow.literal_resolver(self.rdata_pe(b'\x00\x00\x00\x00'), expected)
        actual = function_bytes(self.literal_object(), 'f', 0x401000, lambda n, k: None, literal)
        self.assertNotEqual(actual, expected)

    def test_one_literal_name_keeps_one_address(self):
        pe = self.rdata_pe(b'\x00\x00\x80\x3f\x00\x00\x80\x3f', 0x776a38)
        workflow.literal_resolver(pe, b'\xd9\x05\x38\x6a\x77\x00\xc3')('__real@3f800000', b'\x00\x00\x80\x3f', 2)
        with self.assertRaisesRegex(ValueError, 'elsewhere'):
            workflow.literal_resolver(pe, b'\xd9\x05\x3c\x6a\x77\x00\xc3')('__real@3f800000', b'\x00\x00\x80\x3f', 2)

    def test_literal_without_checker_support_is_rejected(self):
        with self.assertRaisesRegex(ValueError, 'object-local data'):
            function_bytes(self.literal_object(), 'f', 0x401000, lambda n, k: 0x776a3c)


if __name__ == '__main__':
    unittest.main()
