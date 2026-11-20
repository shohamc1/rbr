"""Analyze the pinned EXE or export one function from its saved Ghidra project."""
import argparse
import json
import os
import re
import shutil
from pathlib import Path
import subprocess
import sys

from workflow import entries, reference

ROOT = Path(__file__).resolve().parents[1]
HEADLESS = '/opt/homebrew/opt/ghidra/libexec/support/analyzeHeadless'


def main_checkout():
    """Return the main checkout when ROOT is a linked worktree, else ROOT."""
    try:
        common = subprocess.run(['git', 'rev-parse', '--path-format=absolute', '--git-common-dir'], cwd=ROOT,
                                capture_output=True, text=True, check=True).stdout.strip()
    except (OSError, subprocess.CalledProcessError):
        return ROOT
    return Path(common).parent


def shared_build(relative):
    """Prefer this checkout's build/<relative>; a worktree falls back to the main checkout's copy."""
    local = ROOT / 'build' / relative
    shared = main_checkout() / 'build' / relative
    return local if local.exists() or not shared.exists() else shared


def java_major(home):
    """Return the major version from a JDK's release file, or 0."""
    try:
        text = (Path(home) / 'release').read_text()
    except OSError:
        return 0
    found = re.search(r'JAVA_VERSION="(\d+)', text)
    return int(found.group(1)) if found else 0


def java_minimum(headless):
    """Read the minimum JDK major version from Ghidra's application.properties."""
    props = Path(headless).resolve().parents[1] / 'Ghidra/application.properties'
    try:
        found = re.search(r'^application\.java\.min=(\d+)', props.read_text(), re.M)
    except OSError:
        return 21
    return int(found.group(1)) if found else 21


def installed_version(headless):
    props = Path(headless).resolve().parents[1] / 'Ghidra/application.properties'
    try:
        found = re.search(r'^application\.version=(\S+)', props.read_text(), re.M)
    except OSError:
        return None
    return found.group(1) if found else None


def ghidra_env():
    """Return (headless path, environment) with a JDK that this Ghidra accepts."""
    env = os.environ.copy()
    headless = env.get('GHIDRA_HEADLESS', HEADLESS)
    minimum = java_minimum(headless)
    if 'JAVA_HOME' not in env:
        homes = sorted(Path('/opt/homebrew/opt').glob('openjdk*/libexec/openjdk.jdk/Contents/Home'), key=java_major)
        usable = [h for h in homes if java_major(h) >= minimum]
        if not usable:
            raise ValueError(f'Ghidra needs JDK {minimum} or later; install it with brew install openjdk@{minimum} or set JAVA_HOME')
        env['JAVA_HOME'] = str(usable[0])
    elif java_major(env['JAVA_HOME']) < minimum:
        raise ValueError(f'JAVA_HOME is older than JDK {minimum}, which Ghidra requires: ' + env['JAVA_HOME'])
    env['PATH'] = env['JAVA_HOME'] + '/bin:' + env['PATH']
    return headless, env


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
    if args.command == 'function' and not (project / 'rbr.gpr').exists():
        # Worktrees read the main checkout's saved project; exports stay local.
        project = shared_build(f"ghidra/{cfg['executable_sha256'][:12]}/project/rbr.gpr").parent
    project.mkdir(parents=True, exist_ok=True)
    out.mkdir(parents=True, exist_ok=True)
    headless, env = ghidra_env()
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
    census = shared_build(f"census/{cfg['executable_sha256'][:12]}/census.gpr")
    if (args.command == 'function' and not receipt.exists() and census.exists()
            and 'No function entry' in log.read_text()):
        # The matching project only seeds manifest entries; the census project
        # holds every reviewed boundary candidate.
        print('Not in the matching project; retrying with the census project', flush=True)
        cmd[1:3] = [str(census.parent), 'census']
        with log.open('a') as stream:
            result = subprocess.run(cmd, env=env, stdout=stream, stderr=subprocess.STDOUT)
    if result.returncode or not receipt.exists():
        raise RuntimeError(f'Ghidra failed or did not complete exports; see {log}')
    metadata = json.loads(receipt.read_text())
    if metadata['reference_sha256'].lower() != cfg['executable_sha256']:
        raise ValueError('Export reference hash mismatch')
    if metadata['ghidra_version'] != settings['version']:
        raise ValueError('Ghidra version mismatch; qualify the new version before updating its pin')
    differ = [name for name, expected in settings['analyzers'].items()
              if metadata['analysis_options'].get(name) != expected]
    if differ and cmd[2] == 'census':
        print('WARNING: census project analyzer settings differ from config/ghidra.json: ' + ', '.join(differ)
              + '. Treat this packet as boundary evidence only.')
    elif differ:
        raise ValueError('Ghidra analyzer setting mismatch: ' + differ[0])
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
