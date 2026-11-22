"""Set up and run the pinned MSVC 7.1 (13.10.3077) tools through Docker and Wine."""
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tarfile
import tempfile
import urllib.request
import uuid

from extract import digest

ROOT = Path(__file__).resolve().parent.parent
CONFIG = json.loads((ROOT / 'config/msvc71.json').read_text())
COMPILER = ROOT / 'tools/local/msvc71'
IMAGE_FILE = ROOT / 'tools/local/msvc-wine-image.txt'
TIMEOUT = int(os.environ.get('RBR_MSVC_TIMEOUT', '600'))


def image():
    for name, expected in CONFIG['files'].items():
        if digest(COMPILER / name) != expected:
            raise ValueError(f'MSVC file hash mismatch: {name}; run make setup')
    if not IMAGE_FILE.exists():
        raise ValueError('Docker Wine image is not set up; run make setup')
    image_id = IMAGE_FILE.read_text().strip()
    if not re.fullmatch(r'sha256:[0-9a-f]{64}', image_id):
        raise ValueError('Invalid Docker image ID; run make setup')
    return image_id


def setup():
    COMPILER.parent.mkdir(parents=True, exist_ok=True)
    if not COMPILER.exists():
        with tempfile.TemporaryDirectory(dir=COMPILER.parent) as temp:
            archive = Path(temp) / 'compiler.tar.gz'
            urllib.request.urlretrieve(CONFIG['archive_url'], archive)
            if digest(archive) != CONFIG['archive_sha256']:
                raise ValueError('Compiler archive hash mismatch')
            unpacked = Path(temp) / 'unpacked'
            with tarfile.open(archive) as package:
                package.extractall(unpacked, filter='data')
            unpacked.rename(COMPILER)
    with tempfile.TemporaryDirectory(dir=COMPILER.parent) as temp:
        iid = Path(temp) / 'image.txt'
        subprocess.run(['docker', 'build', '--platform', CONFIG['docker_platform'],
                        '--iidfile', str(iid), str(ROOT / 'tools/docker/msvc')], check=True)
        iid.replace(IMAGE_FILE)
    image()
    print('PASS MSVC hashes and Docker Wine image')


def docker(args, writable, **kwargs):
    """Run the Wine image with the repository mounted read-only at its host path.

    Host paths stay valid inside the container (Z:/Users/...), so the PDB names
    the same source paths that reccmp reads on the host."""
    name = 'rbr-msvc-' + uuid.uuid4().hex[:12]
    command = ['docker', 'run', '--rm', '-i', '--name', name, '--platform', CONFIG['docker_platform'],
               '--network', 'none', '-w', str(ROOT),
               '--mount', f'type=bind,src={ROOT},dst={ROOT},readonly',
               '--mount', f'type=bind,src={COMPILER.resolve()},dst=/compiler,readonly',
               '--mount', f'type=bind,src={Path(writable).resolve()},dst={Path(writable).resolve()}',
               *args]
    try:
        return subprocess.run(command, timeout=TIMEOUT, **kwargs)
    except subprocess.TimeoutExpired:
        subprocess.run(['docker', 'rm', '-f', name], capture_output=True)
        raise ValueError(f'Wine timed out after {TIMEOUT} s; a hang usually means a compile error')


def shell(script, writable):
    """Run a POSIX shell script in the image and return its exit code."""
    return docker(['--entrypoint', 'sh', image(), '-c', script], writable).returncode


if __name__ == '__main__':
    if sys.argv[1:] != ['setup']:
        sys.exit('usage: msvc.py setup')
    setup()
