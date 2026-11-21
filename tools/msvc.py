"""Set up or run the pinned MSVC 13.10.3077 candidate with Docker and Wine."""
import argparse
import json
import os
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import tarfile
import tempfile
import urllib.request
import uuid
from extract import digest


def config():
    return json.loads(Path('config/msvc71.json').read_text())


def verify():
    cfg = config()
    for name, expected in cfg['files'].items():
        path = Path('tools/local/msvc71') / name
        if digest(path) != expected:
            raise ValueError('MSVC file hash mismatch: ' + str(path))
    image_file = Path(cfg['docker_image_file'])
    if not image_file.exists():
        raise ValueError('Docker Wine image is not set up; run make setup-msvc')
    cfg['docker_image'] = image_file.read_text().strip()
    if not re.fullmatch(r'sha256:[0-9a-f]{64}', cfg['docker_image']):
        raise ValueError('Invalid Docker image ID; run make setup-msvc')
    return cfg


def download(url, path, expected):
    urllib.request.urlretrieve(url, path)
    if digest(path) != expected:
        path.unlink()
        raise ValueError('Download hash mismatch: ' + url)


def setup():
    cfg = config()
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
    # Save the image ID only after a successful build; tags can change.
    with tempfile.TemporaryDirectory(dir=local) as temp:
        image_file = Path(temp) / 'image.txt'
        subprocess.run(['docker', 'build', '--platform', cfg['docker_platform'],
                        '--iidfile', str(image_file), 'tools/docker/msvc'], check=True)
        image_file.replace(cfg['docker_image_file'])
    verify()
    print('PASS MSVC compiler hashes and Docker Wine image setup')


TIMEOUT = int(os.environ.get('RBR_MSVC_TIMEOUT', '300'))


def run_wine(cfg, tool, args, output_dir, entrypoint=None, timeout=None):
    # A failed compile can leave Wine waiting forever; name the container so a
    # timeout can remove it instead of leaving the caller to docker rm -f it.
    name = 'rbr-msvc-' + uuid.uuid4().hex[:12]
    command = ['docker', 'run', '--rm', '--name', name, '--platform', cfg['docker_platform'],
               '--network', 'none',
               *(['--entrypoint', entrypoint] if entrypoint else []),
               '--mount', f'type=bind,src={Path.cwd()},dst=/work,readonly',
               '--mount', f'type=bind,src={Path(cfg["compiler"]).resolve().parent},dst=/compiler,readonly',
               '--mount', f'type=bind,src={Path(output_dir).resolve()},dst=/output',
               cfg['docker_image'], *([] if entrypoint else ['/compiler/' + tool]), *args]
    timeout = timeout or TIMEOUT
    try:
        subprocess.run(command, check=True, timeout=timeout)
    except subprocess.TimeoutExpired:
        subprocess.run(['docker', 'rm', '-f', name], capture_output=True)
        raise ValueError(f'{tool} timed out after {timeout} s and its container was removed. '
                         'This usually means a compile error; check the source for syntax errors '
                         'and missing declarations (the image has no standard headers).')


def compile_objects(jobs, output_dir):
    """Compile (source, output, flags) jobs in one container and return {output: log}.

    Each docker run pays about 5 s for container start and Wine prefix setup; a
    warm prefix compiles a leaf in well under a second, so one container with
    parallel cl.exe calls replaces one container per source. A failed job keeps
    no object; its log holds the compiler output.
    """
    cfg = verify()
    output_dir = Path(output_dir)
    work = output_dir / '.batch'
    shutil.rmtree(work, ignore_errors=True)
    work.mkdir(parents=True)
    lines = []
    for index, (source, output, flags) in enumerate(jobs):
        output = Path(output)
        if output.parent.resolve() != output_dir.resolve():
            raise ValueError('Batch outputs must share one directory: ' + str(output))
        output.unlink(missing_ok=True)
        source = Path(source).resolve().relative_to(Path.cwd()).as_posix()
        command = shlex.join(['timeout', str(TIMEOUT), 'wine', '/compiler/cl.exe', *cfg['flags'], *flags,
                              '/FoZ:/output/' + output.name, 'Z:/work/' + source])
        lines.append(f'{command} >/output/.batch/{index}.log 2>&1')
    (work / 'jobs.txt').write_text(''.join(line + '\n' for line in lines))
    workers = min(8, os.cpu_count() or 1)
    script = f'wineboot -i >/dev/null 2>&1; xargs -d "\\n" -P {workers} -n 1 sh -c < /output/.batch/jobs.txt'
    try:
        # Each job has its own timeout; the container limit only catches a stuck Wine server.
        run_wine(cfg, 'cl.exe', ['-c', script], output_dir, entrypoint='sh',
                 timeout=TIMEOUT * (1 + len(jobs) // workers))
    except subprocess.CalledProcessError:
        pass  # xargs exits nonzero when any job fails; each job's log and object tell which.
    logs = {Path(output): (work / f'{index}.log').read_text(errors='replace') if (work / f'{index}.log').exists() else ''
            for index, (_, output, _) in enumerate(jobs)}
    shutil.rmtree(work, ignore_errors=True)
    return logs


def compile_object(source, output, flags=()):
    cfg = verify()
    output = Path(output)
    output.parent.mkdir(parents=True, exist_ok=True)
    output.unlink(missing_ok=True)
    source = Path(source).resolve().relative_to(Path.cwd())
    run_wine(cfg, 'cl.exe', [*cfg['flags'], *flags,
             '/FoZ:/output/' + output.name, 'Z:/work/' + source.as_posix()], output.parent)
    if not output.is_file(): raise ValueError('Compiler did not produce an object')
    return output


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('command', choices=['setup', 'verify', 'link-smoke'])
    args = parser.parse_args()
    if args.command == 'setup': setup()
    elif args.command == 'link-smoke':
        cfg = verify()
        output = Path('build/compiler/link_smoke.exe')
        output.unlink(missing_ok=True)
        obj = compile_object('tests/qualification/msvc71/link_smoke.cpp', 'build/compiler/link_smoke.obj')
        run_wine(cfg, 'link.exe', ['/nologo', '/entry:Entry', '/subsystem:console',
                 '/nodefaultlib', '/fixed', '/out:Z:/output/' + output.name,
                 'Z:/output/' + obj.name], output.parent)
        if not output.exists(): raise ValueError('Linker did not produce an executable')
        import pefile
        from coff import function_bytes
        pe = pefile.PE(str(output))
        code = function_bytes(obj.read_bytes(), '_Entry')
        if pe.FILE_HEADER.Machine != 0x14c or pe.get_data(pe.OPTIONAL_HEADER.AddressOfEntryPoint, len(code)) != code:
            raise ValueError('Linked entry does not match the compiled i386 function')
        print('PASS linked PE32 i386 EXE and compiled entry bytes')
    else:
        verify()
        print('PASS MSVC tool hashes')
