"""Compile every source and link build/RichardBurnsRally.exe with a PDB for reccmp.

MSVC 7.1 compiles through Docker and Wine. LLVM links natively: Wine's emulated
i386 link.exe crashes on a second input file. lld-link ignores the MSVC 7.1
line records, so build/reccmp.csv tells reccmp which linker symbol each
annotation names.

Code and data without source come from config/symbols.csv. Each row becomes a
stub under its linker symbol, and reccmp pairs the stub with its original
address. Imports resolve through import libraries made from the reference
executable's import table. The linked executable is a comparison vehicle; it
does not run.
"""
import csv
import functools
from pathlib import Path
import re
import shlex
import struct
import subprocess
import sys

import pefile
import pydemumble

import msvc

ROOT = msvc.ROOT
BUILD = ROOT / 'build'
OBJ = BUILD / 'obj'
SOURCES = ROOT / 'src'
REFERENCE = ROOT / 'original/nocd/richardburnsrally.exe'
EXE = BUILD / 'RichardBurnsRally.exe'
LLVM = Path('/opt/homebrew/opt/llvm/bin')
LLD_LINK = Path('/opt/homebrew/opt/lld/bin/lld-link')
FLAGS_RE = re.compile(r'^// Build flags: (.+)$', re.M)
ANNOTATION_RE = re.compile(r'^// (?:FUNCTION|STUB|TEMPLATE|SYNTHETIC|LIBRARY): RBR (0x[0-9a-f]+).*\n', re.M)
ALIAS_RE = re.compile(r'/alternatename:([^=\s"]+)=')
MAX_DATA_STUB = 0x1000


def wine_path(path):
    return 'Z:' + str(Path(path).resolve())


def read_coff(path):
    """Return the external symbols and sections of an i386 COFF object.

    symbols: name -> (section number, value, is_function)
    sections: list of (size, raw data offset, [relocation offsets])"""
    data = path.read_bytes()
    _, count, _, symoff, symcount, _, _ = struct.unpack_from('<HHIIIHH', data)
    sections = []
    for index in range(count):
        size, raw, relptr, _, relcount = struct.unpack_from('<IIIIH', data, 20 + index * 40 + 16)
        relocs = [struct.unpack_from('<I', data, relptr + i * 10)[0] for i in range(relcount)]
        sections.append((size, raw, relocs))
    strings = symoff + symcount * 18
    symbols = {}
    index = 0
    while index < symcount:
        raw, value, section, kind, storage, aux = struct.unpack_from('<8sIhHBB', data, symoff + index * 18)
        if raw[:4] == b'\0\0\0\0':
            start = strings + struct.unpack_from('<I', raw, 4)[0]
            name = data[start:data.index(b'\0', start)].decode()
        else:
            name = raw.rstrip(b'\0').decode()
        if storage == 2:  # IMAGE_SYM_CLASS_EXTERNAL
            symbols[name] = (section, value, bool(kind & 0x20))
        index += 1 + aux
    return symbols, sections


def compile_sources():
    OBJ.mkdir(parents=True, exist_ok=True)
    sources = sorted(SOURCES.rglob('*.cpp'))
    headers = max((p.stat().st_mtime for p in SOURCES.rglob('*.h')), default=0)
    jobs = []
    for source in sources:
        obj = OBJ / (source.stem + '.obj')
        if obj.exists() and obj.stat().st_mtime > max(source.stat().st_mtime, headers):
            continue
        obj.unlink(missing_ok=True)
        flags = FLAGS_RE.search(source.read_text())
        command = ['wine', '/compiler/Bin/cl.exe', *msvc.CONFIG['flags'], '/Z7',
                   *(flags[1].split() if flags else []), '/Fo' + wine_path(obj), wine_path(source)]
        jobs.append(f'{shlex.join(command)} > {shlex.quote(str(obj.with_suffix(".log")))} 2>&1')
    if jobs:
        (BUILD / 'jobs.txt').write_text(''.join(job + '\n' for job in jobs))
        msvc.shell('[ -f "$WINEPREFIX/system.reg" ] || wineboot -i >/dev/null 2>&1; '
                   f'xargs -d "\\n" -P 8 -n 1 sh -c < {shlex.quote(str(BUILD / "jobs.txt"))}', BUILD)
    failed = [s for s in sources if not (OBJ / (s.stem + '.obj')).exists()]
    for source in failed:
        log = OBJ / (source.stem + '.log')
        print(log.read_text(errors='replace') if log.exists() else '', file=sys.stderr)
    if failed:
        raise ValueError('Compile failed: ' + ', '.join(str(s.relative_to(ROOT)) for s in failed))
    print(f'compiled {len(jobs)} of {len(sources)} sources')
    return sources


def qualified_name(symbol):
    """Class::method or function name of a linker symbol."""
    if symbol.startswith('?'):
        text = pydemumble.demangle(symbol)
        return text[:text.index('(')].split()[-1].lstrip('*&') if '(' in text else symbol
    return re.sub(r'@\d+$', '', symbol[1:])


def annotated_functions(sources):
    """Yield (address, source, object, symbol) for each function annotation."""
    for source in sources:
        obj = OBJ / (source.stem + '.obj')
        symbols, _ = read_coff(obj)
        names = {}
        for symbol, (section, _, is_function) in symbols.items():
            if section > 0 and is_function:
                names.setdefault(qualified_name(symbol), []).append(symbol)
        text = source.read_text()
        for match in ANNOTATION_RE.finditer(text):
            head = text[match.end():text.index('(', match.end())]
            name = head.split()[-1].lstrip('*&')
            found = names.get(name, [])
            if len(found) != 1:
                raise ValueError(f'{source.relative_to(ROOT)}: annotation {match[1]} names {name}, '
                                 f'which matches {len(found)} defined functions')
            yield int(match[1], 16), source, obj, found[0]


def symbol_rows():
    with (ROOT / 'config/symbols.csv').open(newline='') as stream:
        return list(csv.DictReader(stream))


@functools.cache
def reference():
    return pefile.PE(str(REFERENCE), fast_load=True)


def writable(address):
    rva = address - reference().OPTIONAL_HEADER.ImageBase
    return any(s.contains_rva(rva) and s.Characteristics & 0x80000000 for s in reference().sections)


def write_stubs(rows):
    """Assemble stubs for config/symbols.csv and return (object, data sizes).

    Rows that share an address become aliases of one stub. A data stub takes
    its row's size. Without one, a writable data stub spans the gap to the
    next listed address, and the reference gets the same extent, so a member
    offset names the same variable in both images. Other stubs stay one word:
    reccmp copies the extent to the reference, and strings follow the vtables
    in read-only data."""
    groups = {}
    for row in rows:
        groups.setdefault(int(row['address'], 16), []).append(row)
    addresses = sorted(groups)
    text = ['.text', '.globl _rbrEntry', '_rbrEntry:', 'ret']
    data = ['.data', '.globl __fltused', '__fltused:', '.long 0']  # CRT floating-point marker
    sizes = {}
    for index, address in enumerate(addresses):
        code = groups[address][0]['type'] in ('stub', 'library')
        target = text if code else data
        for row in groups[address]:
            target += [f'.globl "{row["symbol"]}"', f'"{row["symbol"]}":']
        if code:
            target.append('int3')
        else:
            following = addresses[index + 1] if index + 1 < len(addresses) else address + 4
            size = max(int(row['size'] or 0) for row in groups[address])
            if not size and writable(address):
                size = sizes[address] = max(4, min(following - address, MAX_DATA_STUB))
            target.append(f'.space {size or 4}')
    source = BUILD / 'stubs.s'
    source.write_text('\n'.join(text + data) + '\n')
    obj = source.with_suffix('.obj')
    subprocess.run([LLVM / 'llvm-mc', '-triple', 'i686-pc-win32', '-filetype=obj', '-o', obj, source], check=True)
    return obj, sizes


def import_libraries(undefined):
    """Build import libraries for the imports the objects use, from the reference."""
    pe = reference()
    pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY['IMAGE_DIRECTORY_ENTRY_IMPORT']])
    owners = {entry.name.decode(): descriptor.dll.decode()
              for descriptor in pe.DIRECTORY_ENTRY_IMPORT for entry in descriptor.imports if entry.name}
    exports, missing = {}, []
    for symbol in sorted(undefined):
        name = symbol.removeprefix('__imp_')
        plain = re.sub(r'@\d+$', '', name[1:]) if name.startswith('_') else name
        if plain not in owners:
            missing.append(symbol)
            continue
        # dlltool adds the C underscore back; -k imports a stdcall name without @N.
        exports.setdefault(owners[plain], set()).add(name[1:] if name.startswith('_') else name)
    if missing:
        raise ValueError('Unresolved symbols; define them in source or add them to '
                         'config/symbols.csv:\n  ' + '\n  '.join(missing))
    libs = []
    (BUILD / 'lib').mkdir(exist_ok=True)
    for dll, names in exports.items():
        definition = BUILD / 'lib' / (Path(dll).stem + '.def')
        definition.write_text(f'LIBRARY {dll}\nEXPORTS\n' + ''.join(f'    {n}\n' for n in sorted(names)))
        libs.append(definition.with_suffix('.lib'))
        subprocess.run([LLVM / 'llvm-dlltool', '-m', 'i386', '-k', '-d', definition, '-l', libs[-1]],
                       check=True)
    return libs


def link(sources):
    objects = [OBJ / (s.stem + '.obj') for s in sources]
    rows = symbol_rows()
    defined, undefined = set(), set()
    for obj in objects:
        for name, (section, value, _) in read_coff(obj)[0].items():
            (defined if section > 0 or value else undefined).add(name)
    stubbed = {row['symbol'] for row in rows}
    if clash := sorted(stubbed & defined):
        raise ValueError('Source now defines these config/symbols.csv rows; delete them:\n  '
                         + '\n  '.join(clash))
    # /alternatename pragmas in source bind a name to another definition.
    aliased = {m for s in sources for m in ALIAS_RE.findall(s.read_text())}
    libs = import_libraries(undefined - defined - stubbed - aliased - {'__fltused'})
    stubs, sizes = write_stubs(rows)
    functions = list(annotated_functions(sources))
    with (BUILD / 'reccmp.csv').open('w', newline='') as stream:
        writer = csv.writer(stream, lineterminator='\n')
        writer.writerow(['address', 'symbol', 'size'])
        writer.writerows((f'0x{a:08x}', symbol, '') for a, _, _, symbol in functions)
        writer.writerows((f'0x{a:08x}', '', size) for a, size in sizes.items())
    with (BUILD / 'functions.csv').open('w', newline='') as stream:
        writer = csv.writer(stream, lineterminator='\n')
        writer.writerow(['address', 'source', 'object', 'symbol'])
        writer.writerows((f'0x{a:08x}', s.relative_to(ROOT), o.relative_to(ROOT), symbol)
                         for a, s, o, symbol in functions)
    EXE.unlink(missing_ok=True)
    result = subprocess.run(
        [LLD_LINK, '/nologo', '/machine:x86', '/subsystem:windows', '/entry:rbrEntry', '/nodefaultlib',
         '/debug', '/opt:noref,noicf', '/incremental:no', '/safeseh:no', '/base:0x400000',
         f'/out:{EXE}', f'/pdb:{EXE.with_suffix(".pdb")}', stubs, *objects, *libs],
        capture_output=True, text=True)
    if result.returncode:
        raise ValueError('\n'.join(line for line in result.stdout.splitlines() if 'warning:' not in line))
    (BUILD / 'reccmp-build.yml').write_text(
        f'project: {ROOT}\ntargets:\n  RBR:\n    path: {EXE.name}\n    pdb: {EXE.with_suffix(".pdb").name}\n')
    print('linked', EXE.relative_to(ROOT))


def main():
    link(compile_sources())


if __name__ == '__main__':
    try:
        main()
    except (ValueError, OSError) as error:
        sys.exit(str(error))
