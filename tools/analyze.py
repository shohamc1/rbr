"""Export PE metadata and bounded function evidence; linear xrefs are candidates."""
import json
from pathlib import Path
import struct
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from workflow import reference, entries, original_bytes
from extract import digest


def save(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2) + '\n')


def main():
    cfg, path, pe = reference()
    out = Path('analysis')
    modules = []
    for name in cfg['reconstruct'] + cfg['secondary_variants']:
        module = Path(name)
        image = pefile.PE(str(module))
        base = image.OPTIONAL_HEADER.ImageBase
        modules.append({'path':str(module), 'sha256':digest(module), 'image_base':hex(base),
          'entry':hex(base + image.OPTIONAL_HEADER.AddressOfEntryPoint),
          'linker_version':[image.OPTIONAL_HEADER.MajorLinkerVersion, image.OPTIONAL_HEADER.MinorLinkerVersion],
          'sections':[{'name':s.Name.rstrip(b'\0').decode(), 'va':hex(base+s.VirtualAddress), 'virtual_size':s.Misc_VirtualSize, 'file_offset':s.PointerToRawData, 'raw_size':s.SizeOfRawData, 'flags':hex(s.Characteristics)} for s in image.sections],
          'imports':[{'dll':imp.dll.decode(), 'symbols':[{'name':x.name.decode() if x.name else None, 'ordinal':x.ordinal, 'iat':hex(x.address)} for x in imp.imports]} for imp in getattr(image,'DIRECTORY_ENTRY_IMPORT',[])],
          'exports':[{'name':x.name.decode() if x.name else None,'ordinal':x.ordinal,'va':hex(base+x.address)} for x in getattr(getattr(image,'DIRECTORY_ENTRY_EXPORT',None),'symbols',[])]})
    save(out/'modules.json',modules)
    rows = entries()
    targets = {int(r['address'],16):[] for r in rows}
    md=Cs(CS_ARCH_X86,CS_MODE_32); md.skipdata=True
    for section in pe.sections:
        if not section.Characteristics & 0x20000000: continue
        for address,size,mnemonic,operands in md.disasm_lite(section.get_data()[:section.Misc_VirtualSize],pe.OPTIONAL_HEADER.ImageBase+section.VirtualAddress):
            if mnemonic in ('call','jmp') and operands.startswith('0x'):
                target=int(operands,16)
                if target in targets: targets[target].append({'instruction':hex(address),'kind':mnemonic})
    md.skipdata=False
    for row in rows:
        address=int(row['address'],16); offset,body=original_bytes(pe,row)
        pointers=[]
        needle=struct.pack('<I',address)
        for section in pe.sections:
            if section.Characteristics & 0x20000000: continue
            data=section.get_data(); pos=0
            while (pos:=data.find(needle,pos))>=0:
                pointers.append(hex(pe.OPTIONAL_HEADER.ImageBase+section.VirtualAddress+pos));pos+=1
        save(out/'functions'/(row['id']+'.json'),{'id':row['id'],'reference_sha256':cfg['executable_sha256'],'address':row['address'],'size':row['size'],'file_offset':offset,'bytes':body.hex(' '),'boundary_confidence':row['boundary_confidence'],'instructions':[f'{i.address:08x}: {i.mnemonic} {i.op_str}' for i in md.disasm(body,address)],'direct_xref_candidates':targets[address],'data_pointer_candidates':pointers,'limitations':'Linear decoding can misidentify embedded data; these are reference sites, not proved caller boundaries. Indirect callers are incomplete. No calls or global-data accesses occur in these straight-line leaf bodies.'})
    print('Wrote analysis/modules.json and bounded function evidence')


if __name__=='__main__': main()
