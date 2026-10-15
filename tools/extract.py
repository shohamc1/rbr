"""Extract the pinned local media without executing its installers."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess


def digest(path):
    h = hashlib.sha256()
    with Path(path).open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            h.update(block)
    return h.hexdigest()


def carve(data):
    pattern = rb'(data[12]\.(?:cab|hdr))\x00Disk1\\\1\x000\.0\.0\.0\x00([0-9]+)\x00'
    files = {}
    for match in re.finditer(pattern, data):
        name, size = match[1].decode(), int(match[2])
        blob = data[match.end():match.end() + size]
        if name in files or len(blob) != size or blob[:4] != b'ISc(':
            raise ValueError('Invalid or duplicate embedded cabinet: ' + name)
        files[name] = blob
    if set(files) != {'data1.cab', 'data1.hdr', 'data2.cab'}:
        raise ValueError('Expected three InstallShield cabinet members')
    return files


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('iso', type=Path)
    args = parser.parse_args()
    expected = json.loads(Path('config/reference.json').read_text())['iso_sha256']
    if digest(args.iso) != expected:
        raise ValueError('ISO hash differs from the selected reference')
    root = Path('original')
    if root.exists():
        raise ValueError('original/ already exists. Preserve it; use a fresh checkout for extraction.')
    (root / 'disc').mkdir(parents=True)
    subprocess.run(['bsdtar', '-xf', str(args.iso.resolve()), '-C', str(root / 'disc')], check=True)
    subprocess.run(['unshield', '-d', str(root / 'install'), 'x', str(root / 'disc/data1.cab')], check=True)
    for version in ('1.01', '1.02'):
        installer = root / f'disc/Crack/Update {version}/Richard Burns Rally {version} Patch.exe'
        out = root / ('patch' + version)
        out.mkdir()
        for name, blob in carve(installer.read_bytes()).items():
            (out / name).write_bytes(blob)
        subprocess.run(['unshield', '-d', str(out / 'files'), 'x', str(out / 'data1.cab')], check=True)
    (root / 'nocd').mkdir()
    shutil.copy2(root / 'disc/Crack/richardburnsrally.exe', root / 'nocd/richardburnsrally.exe')
    print('Extracted base media and both patches separately. No installer was executed.')


if __name__ == '__main__':
    main()
