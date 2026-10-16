"""Read one function from a small i386 COFF object, resolving declared relocations."""
import struct

REL32 = 20  # IMAGE_REL_I386_REL32
DIR32 = 6   # IMAGE_REL_I386_DIR32


def function_bytes(data, symbol, func_va=None, resolve=None):
    machine, count, _, symoff, symcount, optional, _ = struct.unpack_from('<HHIIIHH', data)
    if machine != 0x14c or optional != 0:
        raise ValueError('Expected an i386 COFF object')
    sections = []
    for index in range(count):
        off = 20 + index * 40
        fields = struct.unpack_from('<8sIIIIIIHHI', data, off)
        name = fields[0].rstrip(b'\0')
        size, raw, relptr, relcount, flags = fields[3], fields[4], fields[5], fields[7], fields[9]
        if raw + size > len(data):
            raise ValueError('Truncated section')
        sections.append((name, data[raw:raw + size], relptr, relcount, flags))
    strings = symoff + symcount * 18
    if strings + 4 > len(data):
        raise ValueError('Truncated symbol table')
    symbols = []
    index = 0
    while index < symcount:
        name, value, section, kind, storage, aux = struct.unpack_from('<8sIhHBB', data, symoff + index * 18)
        if name[:4] == b'\0' * 4:
            start = strings + struct.unpack_from('<I', name, 4)[0]
            end = data.index(b'\0', start)
            name = data[start:end]
        else:
            name = name.rstrip(b'\0')
        symbols.append((name.decode(), value, section, kind, storage))
        symbols.extend([None] * aux)
        index += 1 + aux
    found = [s for s in symbols if s and s[4] == 2 and s[3] & 0x20 and s[2] > 0]
    if len(found) != 1 or found[0][0] != symbol or found[0][1] != 0:
        raise ValueError(f'Expected only function {symbol} at section offset zero; got {found}')
    _, _, section, _, _ = found[0]
    name, code, relptr, relcount, flags = sections[section - 1]
    if name != b'.text' or not flags & 0x20:
        raise ValueError('Expected .text code')
    for index, (other, body, _, _, other_flags) in enumerate(sections, 1):
        if index != section and body and other_flags & (0x20 | 0x40 | 0x80) and not other.startswith(b'.debug'):
            raise ValueError('Additional code/data needs a linker-aware checker: ' + repr(other))
    if relcount:
        if func_va is None or resolve is None:
            raise ValueError('Relocations need checker support with known symbol addresses: ' + symbol)
        patched = bytearray(code)
        if relptr + relcount * 10 > len(data):
            raise ValueError('Truncated relocation table')
        for index in range(relcount):
            offset, symidx, kind = struct.unpack_from('<IIH', data, relptr + index * 10)
            if offset + 4 > len(code):
                raise ValueError('Relocation is outside function bytes')
            if not 0 <= symidx < len(symbols) or symbols[symidx] is None:
                raise ValueError('Relocation targets an invalid symbol table entry')
            target_name, value, target_section, _, storage = symbols[symidx]
            if target_section != 0 or storage != 2 or value != 0:
                raise ValueError('Relocation target must be an undefined external: ' + target_name)
            target = resolve(target_name, kind)
            if target is None:
                raise ValueError('Unresolved relocation to ' + target_name)
            if kind == REL32:
                addend = struct.unpack_from('<i', code, offset)[0]
                displacement = target + addend - (func_va + offset + 4)
                if not -0x80000000 <= displacement <= 0x7fffffff:
                    raise ValueError('REL32 displacement out of range to ' + target_name)
                struct.pack_into('<i', patched, offset, displacement)
            elif kind == DIR32:
                if not 0 <= target <= 0xffffffff:
                    raise ValueError('DIR32 target out of range to ' + target_name)
                addend = struct.unpack_from('<I', code, offset)[0]
                struct.pack_into('<I', patched, offset, (target + addend) & 0xffffffff)
            else:
                raise ValueError(f'Unsupported relocation type {kind} to ' + target_name)
        code = bytes(patched)
    return code
