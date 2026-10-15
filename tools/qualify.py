"""Test narrow Clang candidates against original bodies; report limits."""
import json
from pathlib import Path
import subprocess
from coff import function_bytes
from workflow import reference, compiler, entries, check, difference, BUILD


def main():
    cfg, reference_path, pe = reference()
    cc, flags = compiler()
    BUILD.mkdir(parents=True, exist_ok=True)
    for row in entries():
        if row['status'] == 'matching_cpp': check(row,pe)
    results=[]
    for name,address,size,symbol,extra in [
        ('constructor',0x406410,26,'??0Pair@@QAE@MM@Z',['-msse','-mno-sse2']),
        ('predicate',0x401020,11,'?present@Object@@QAEHXZ',[])]:
        out=BUILD/(name+'.obj')
        out.unlink(missing_ok=True)
        subprocess.run([cc,*flags,*extra,'-c',f'tests/qualification/{name}.cpp','-o',str(out)],check=True)
        actual=function_bytes(out.read_bytes(),symbol)
        expected=pe.get_data(address-pe.OPTIONAL_HEADER.ImageBase,size)
        results.append({'name':name,'address':hex(address),'size':size,'extra_flags':extra,'match':actual==expected,'difference':difference(expected,actual,address)})
    report={'reference':str(reference_path),'reference_sha256':cfg['executable_sha256'],'profile':'config/toolchain.json','verified_leaf_examples':3,'additional_candidates':results,'limits':'No general compiler qualification. No linked game, virtual dispatch, RTTI, exceptions, runtime floating-point environment, or original linker reproduction.'}
    Path('build/reconstruction/qualification.json').write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps(report,indent=2))
    if any(not row['match'] for row in results):
        raise SystemExit(1)


if __name__=='__main__':main()
