"""Read one relocation-free function from a small i386 COFF object."""
import struct


def function_bytes(data, symbol):
    machine, count, _, symoff, symcount, optional, _ = struct.unpack_from('<HHIIIHH', data)
    if machine != 0x14c or optional != 0:
        raise ValueError('Expected an i386 COFF object')
    sections = []
    for index in range(count):
        off = 20 + index * 40
        name, _, _, size, raw, _, _, relocations, _, flags = struct.unpack_from('<8sIIIIIIHHI', data, off)
        if raw + size > len(data):
            raise ValueError('Truncated section')
        sections.append((name.rstrip(b'\0'), data[raw:raw + size], relocations, flags))
    strings = symoff + symcount * 18
    if strings + 4 > len(data):
        raise ValueError('Truncated symbol table')
    found = []
    index = 0
    while index < symcount:
        name, value, section, kind, storage, aux = struct.unpack_from('<8sIhHBB', data, symoff + index * 18)
        if name[:4] == b'\0' * 4:
            start = strings + struct.unpack_from('<I', name, 4)[0]
            end = data.index(b'\0', start)
            name = data[start:end]
        else:
            name = name.rstrip(b'\0')
        if storage == 2 and kind & 0x20 and section > 0:
            found.append((name.decode(), value, section))
        index += 1 + aux
    if len(found) != 1 or found[0][0] != symbol or found[0][1] != 0:
        raise ValueError(f'Expected only function {symbol} at section offset zero; got {found}')
    _, _, section = found[0]
    name, code, relocations, flags = sections[section - 1]
    if name != b'.text' or not flags & 0x20 or relocations:
        raise ValueError('Expected .text code with no unresolved relocations')
    for index, (name, body, _, flags) in enumerate(sections, 1):
        if index != section and body and flags & (0x20 | 0x40 | 0x80) and not name.startswith(b'.debug'):
            raise ValueError('Additional code/data needs a linker-aware checker: ' + repr(name))
    return code
