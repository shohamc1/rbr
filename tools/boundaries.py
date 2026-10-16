"""Find x86 MSVC RTTI-backed virtual tables. Results remain analysis candidates."""
from bisect import bisect_right, insort
import struct
import capstone as cs


def leaf_size(code, address):
    """Recognize a bounded straight-line leaf followed by INT3 alignment."""
    md = cs.Cs(cs.CS_ARCH_X86, cs.CS_MODE_32)
    md.detail = True
    for instruction in md.disasm(code[:256], address):
        if instruction.mnemonic in ('ret', 'retf'):
            if instruction.mnemonic != 'ret':
                return None
            end = instruction.address + instruction.size
            stop = (end + 15) & ~15
            padding = code[end - address:stop - address]
            if padding and len(padding) == stop - end and all(b == 0xcc for b in padding):
                return end - address
            return None
        if instruction.mnemonic in ('int3', 'hlt', 'ud2') or any(instruction.group(group) for group in
                (cs.CS_GRP_JUMP, cs.CS_GRP_CALL, cs.CS_GRP_INT, cs.CS_GRP_IRET, cs.CS_GRP_PRIVILEGE)):
            return None
    return None


def find_vtables(image, base, data_ranges, code_ranges, stops=()):
    def contains(ranges, address, size=1):
        return any(lo <= address and address + size <= hi for lo, hi in ranges)

    def word(address):
        if not contains(data_ranges, address, 4):
            raise ValueError('Outside backed data')
        return struct.unpack_from('<I', image, address - base)[0]

    tables = []
    for lo, hi in data_ranges:
        for slot in range((lo + 3) & ~3, hi - 3, 4):
            locator = word(slot)
            if not contains(data_ranges, locator, 20):
                continue
            # MSVC x86 CompleteObjectLocator and ClassHierarchyDescriptor.
            if word(locator) != 0:
                continue
            type_address = word(locator + 12)
            hierarchy = word(locator + 16)
            if not contains(data_ranges, type_address, 12) or not contains(data_ranges, hierarchy, 16):
                continue
            name_offset = type_address - base + 8
            if image[name_offset:name_offset + 4] not in (b'.?AV', b'.?AU'):
                continue
            if word(hierarchy) != 0 or word(hierarchy + 4) & ~7:
                continue
            count = word(hierarchy + 8)
            if count == 0 or not contains(data_ranges, word(hierarchy + 12), 4 * count):
                continue
            target_slot = slot + 4
            targets = []
            while contains(data_ranges, target_slot, 4):
                if any(lo <= target_slot < hi for lo, hi in stops):
                    break
                target = word(target_slot)
                if not contains(code_ranges, target):
                    break
                targets.append(target)
                target_slot += 4
            if targets:
                end = image.find(b'\0', name_offset, name_offset + 4096)
                if end < 0:
                    continue
                tables.append({'address': hex(slot + 4), 'locator': hex(locator),
                               'type_descriptor': hex(type_address),
                               'type_name': image[name_offset:end].decode('ascii', errors='replace'),
                               'targets': [f'{x:08x}' for x in targets]})
    return tables


def discovery(pe, report, reviewed_stops=()):
    base = pe.OPTIONAL_HEADER.ImageBase
    code = []
    data = []
    for section in pe.sections:
        start = base + section.VirtualAddress
        size = min(section.Misc_VirtualSize, section.SizeOfRawData)
        (code if section.Characteristics & 0x20000000 else data).append((start, start + size))
    image = pe.get_memory_mapped_image()
    stops = [(int(s['start'], 16), int(s['end_inclusive'], 16) + 1)
             for s in report.get('string_ranges', [])]
    stops += [(a, a + 4) for a in reviewed_stops]
    tables = find_vtables(image, base, data, code, stops)
    entries = {int(f['address'], 16) for f in report['functions']}
    bodies = sorted((int(r['start'], 16), int(r['end_inclusive'], 16) + 1)
                    for f in report['functions'] for r in f['ranges'])
    starts = [r[0] for r in bodies]

    def owned(address):
        pos = bisect_right(starts, address) - 1
        return pos >= 0 and address < bodies[pos][1]

    targets = {int(t, 16) for table in tables for t in table['targets']}
    # Interior targets are review items. Never split an existing body automatically.
    missing = targets - entries
    seeds = sorted(t for t in missing if not owned(t))
    padding = []
    returns = []
    return_successors = set(report.get('aligned_return_successors', []))
    leaves = []
    for lo, hi in code:
        for address in range((lo + 15) & ~15, hi, 16):
            if address in entries or owned(address) or address == lo:
                continue
            offset = address - base
            if f'{address:08x}' in return_successors and image[offset] not in (0xcc, 0x90, 0):
                returns.append(f'{address:08x}')
            if image[offset - 1] == 0xcc and image[offset] not in (0xcc, 0x90, 0):
                padding.append(f'{address:08x}')
                size = leaf_size(image[offset:min(hi - base, offset + 272)], address)
                if size is not None and not any(owned(a) for a in range(address, address + size)):
                    leaves.append({'address': f'{address:08x}', 'size': size})
    cfg_seeds = []
    candidates = sorted({int(a, 16) for a in padding + returns})
    known_entries = entries | set(candidates)
    limits = sorted(known_entries)
    for address in candidates:
        next_entry = bisect_right(limits, address)
        limit = min(address + 4096, limits[next_entry] if next_entry < len(limits) else address + 4096)
        limit = min(limit, next(hi for lo, hi in code if lo <= address < hi))
        candidate = bounded_cfg(image[address - base:limit - base], address, known_entries)
        if candidate and not any(owned(a) for a in range(address, address + candidate['size'])):
            cfg_seeds.append(candidate)
            successor = candidate['aligned_return_successor']
            if (successor is not None and successor not in known_entries and not owned(successor) and
                    any(lo <= successor < hi for lo, hi in code) and image[successor - base] not in (0xcc, 0x90, 0)):
                returns.append(f'{successor:08x}')
                known_entries.add(successor)
                insort(limits, successor)
                candidates.append(successor)
    return {'sha256': report['sha256'], 'vtables': tables,
            'distinct_vtable_targets': len(targets),
            'vtable_targets_in_existing_body_interiors': [f'{t:08x}' for t in sorted(missing - set(seeds))],
            'vtable_seeds': [f'{t:08x}' for t in seeds],
            'padded_leaf_seeds': leaves, 'bounded_cfg_seeds': cfg_seeds,
            'padding_entry_candidates': padding, 'return_entry_candidates': returns,
            'limitations': 'RTTI and padding supply evidence, not a proof of callable entry or final boundary. Only complete bounded graphs or straight-line leaves are auto-created.'}


def bounded_cfg(code, address, entries):
    """Accept a bounded code candidate only when all paths and bytes are accounted for."""
    md = cs.Cs(cs.CS_ARCH_X86, cs.CS_MODE_32)
    md.detail = True
    pending = [address]
    decoded = {}
    covered = set()
    exits = 0
    while pending:
        pc = pending.pop()
        if pc in decoded:
            continue
        offset = pc - address
        if offset < 0 or offset >= len(code) or offset in covered:
            return None
        instruction = next(md.disasm(code[offset:offset + 15], pc, count=1), None)
        if instruction is None:
            return None
        span = set(range(offset, offset + instruction.size))
        if span & covered or offset + instruction.size > len(code):
            return None
        decoded[pc] = instruction
        covered.update(span)
        end = pc + instruction.size
        if instruction.mnemonic == 'ret':
            exits += 1
            continue
        if instruction.mnemonic in ('int3', 'hlt', 'ud2', 'retf') or any(
                instruction.group(g) for g in (cs.CS_GRP_INT, cs.CS_GRP_IRET, cs.CS_GRP_PRIVILEGE)):
            return None
        if instruction.group(cs.CS_GRP_JUMP):
            if not instruction.operands or instruction.operands[0].type != cs.x86.X86_OP_IMM:
                return None
            target = instruction.operands[0].imm
            if instruction.mnemonic == 'jmp' and target in entries and not address <= target < address + len(code):
                exits += 1
            else:
                pending.append(target)
            if instruction.mnemonic == 'jmp':
                continue
        if instruction.group(cs.CS_GRP_CALL) and instruction.operands[0].type == cs.x86.X86_OP_IMM:
            if instruction.operands[0].imm not in entries:
                return None
        pending.append(end)
    if not exits:
        return None
    size = max(covered) + 1
    stop = (address + size + 15) & ~15
    padding = code[size:stop - address]
    if len(padding) != stop - address - size or any(b != 0xcc for b in padding):
        return None
    # Reject gaps that could hide data or a second entry. Branches after RET stay in the work list.
    if any(i not in covered for i in range(size)):
        return None
    end = address + size
    last = decoded[max(decoded)]
    return {'address': f'{address:08x}', 'size': size,
            'instruction_count': len(decoded), 'exits': exits,
            'aligned_return_successor': end if last.mnemonic == 'ret' and end % 16 == 0 else None}
