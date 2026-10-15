"""Set up or run the pinned MSVC 13.10.3077 candidate on the tested macOS host."""
import argparse
import json
from pathlib import Path
import platform
import subprocess
import tarfile
import tempfile
import urllib.request
from extract import digest


def config():
    return json.loads(Path('config/msvc71.json').read_text())


def verify():
    cfg = config()
    if platform.system() != cfg['host']:
        raise ValueError('This runner pin is tested on macOS only; qualify a host-specific runner first')
    for name, expected in cfg['files'].items():
        path = Path('tools/local/msvc71') / name
        if digest(path) != expected:
            raise ValueError('MSVC file hash mismatch: ' + str(path))
    if digest(cfg['runner']) != cfg['runner_sha256']:
        raise ValueError('MSVC runner hash mismatch')
    return cfg


def download(url, path, expected):
    urllib.request.urlretrieve(url, path)
    if digest(path) != expected:
        path.unlink()
        raise ValueError('Download hash mismatch: ' + url)


def setup():
    cfg = config()
    if platform.system() != cfg['host']:
        raise ValueError('Only the macOS runner is pinned')
    local = Path('tools/local')
    local.mkdir(parents=True, exist_ok=True)
    destination = local / 'msvc71'
    if not destination.exists():
        with tempfile.TemporaryDirectory(dir=local) as temp:
            archive = Path(temp) / 'compiler.tar.gz'
            download(cfg['archive_url'], archive, cfg['archive_sha256'])
            unpacked = Path(temp) / 'unpacked'
            with tarfile.open(archive) as package:
                package.extractall(unpacked, filter='data')
            for name, expected in cfg['files'].items():
                if digest(unpacked / name) != expected:
                    raise ValueError('Extracted compiler hash mismatch: ' + name)
            unpacked.rename(destination)
    runner = Path(cfg['runner'])
    if not runner.exists():
        download(cfg['runner_url'], runner, cfg['runner_sha256'])
        runner.chmod(0o755)
    verify()
    print('PASS MSVC compiler files and macOS wibo runner hashes')


def compile_object(source, output, flags=()):
    cfg = verify()
    output = Path(output)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.unlink(missing_ok=True)
    subprocess.run([cfg['runner'], cfg['compiler'], *cfg['flags'], *flags,
                    '/Fo' + str(output), str(source)], check=True)
    return output


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('command', choices=['setup', 'verify', 'link-smoke'])
    args = parser.parse_args()
    if args.command == 'setup': setup()
    elif args.command == 'link-smoke':
        cfg = verify()
        obj = compile_object('tests/qualification/msvc71/link_smoke.cpp', 'build/compiler/link_smoke.obj')
        output = Path('build/compiler/link_smoke.exe')
        output.unlink(missing_ok=True)
        subprocess.run([cfg['runner'], str(Path(cfg['compiler']).with_name('link.exe')),
                        '/nologo', '/entry:Entry', '/subsystem:console', '/nodefaultlib',
                        '/fixed', '/out:' + str(output), str(obj)], check=True)
        if not output.exists(): raise ValueError('Linker did not produce an executable')
    else:
        verify()
        print('PASS MSVC tool hashes')
