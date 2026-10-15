"""Analyze the pinned EXE or export one function from its saved Ghidra project."""
import argparse
import json
import os
import shutil
from pathlib import Path
import subprocess
import sys

from workflow import entries, reference

ROOT = Path(__file__).resolve().parents[1]


def address(value):
    value = value.removeprefix('sub_')
    number = int(value, 16)
    if not 0 <= number <= 0xffffffff:
        raise ValueError('Expected a 32-bit hexadecimal address')
    return f'{number:08x}'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('command', choices=['analyze', 'function'])
    parser.add_argument('address', nargs='?')
    args = parser.parse_args()
    cfg, exe, _ = reference()
    settings = json.loads((ROOT / 'config/ghidra.json').read_text())
    base = ROOT / 'build/ghidra' / cfg['executable_sha256'][:12]
    project = base / 'project'
    out = base / 'exports'
    project.mkdir(parents=True, exist_ok=True)
    out.mkdir(parents=True, exist_ok=True)
    headless = os.environ.get('GHIDRA_HEADLESS', '/opt/homebrew/opt/ghidra/libexec/support/analyzeHeadless')
    env = os.environ.copy()
    env.setdefault('JAVA_HOME', '/opt/homebrew/opt/openjdk@21/libexec/openjdk.jdk/Contents/Home')
    env['PATH'] = env['JAVA_HOME'] + '/bin:' + env['PATH']
    cmd = [headless, str(project), 'rbr', '-scriptPath', str(ROOT / 'tools/ghidra'), '-max-cpu', '4']
    if args.command == 'analyze':
        shutil.rmtree(out)
        out.mkdir()
        rows = entries()
        seed = base / 'seeds.tsv'
        seed.write_text(''.join(f"{address(r['address'])}\t{r['size']}\t{r['id']}\n" for r in rows))
        targets = [address(r['address']) for r in rows]
        cmd += ['-import', str(exe.resolve()), '-overwrite', '-processor', settings['language'], '-cspec', settings['compiler_spec'],
                '-preScript', 'SeedFunctions.java', str(seed), str(ROOT / 'config/ghidra.json'), '-analysisTimeoutPerFile', '600']
    else:
        if not args.address:
            parser.error('function requires an address or sub_<address>')
        if not (project / 'rbr.gpr').exists():
            parser.error('Run make ghidra-analyze first')
        targets = [address(args.address)]
        cmd += ['-process', exe.name, '-readOnly', '-noanalysis']
    cmd += ['-postScript', 'ExportFunctions.java', str(out), cfg['executable_sha256'], *targets]
    receipt = out / 'complete.json'
    receipt.unlink(missing_ok=True)
    for target in targets:
        (out / (target + '.json')).unlink(missing_ok=True)
    log = base / (args.command + '.log')
    print(f'Ghidra {args.command}; log: {log}', flush=True)
    with log.open('w') as stream:
        result = subprocess.run(cmd, env=env, stdout=stream, stderr=subprocess.STDOUT)
    if result.returncode or not receipt.exists():
        raise RuntimeError(f'Ghidra failed or did not complete exports; see {log}')
    metadata = json.loads(receipt.read_text())
    if metadata['reference_sha256'].lower() != cfg['executable_sha256']:
        raise ValueError('Export reference hash mismatch')
    if metadata['ghidra_version'] != settings['version']:
        raise ValueError('Ghidra version mismatch; qualify the new version before updating its pin')
    for name, expected in settings['analyzers'].items():
        if metadata['analysis_options'].get(name) != expected:
            raise ValueError('Ghidra analyzer setting mismatch: ' + name)
    for target in targets:
        path = out / (target + '.json')
        row = json.loads(path.read_text())
        if row['address'] != target or not row['decompile_completed']:
            raise ValueError('Incomplete or wrong function export: ' + target)
        print(path)
    print(f'Function index: {out / "functions.json"}')


if __name__ == '__main__':
    os.chdir(ROOT)
    try:
        main()
    except (OSError, ValueError, RuntimeError) as error:
        sys.exit(str(error))
