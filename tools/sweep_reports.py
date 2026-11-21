"""Integrator sweep: apply subagent shared-file requests from build/harness/reports/*.jsonl.

Each line: {"fn": "sub_xxxxxxxx", "kind": "symbol|flags|global|callee", ...}.
Applied lines are moved to build/harness/reports/applied.jsonl. Idempotent:
duplicate globals (by address) and duplicate manifest ids are skipped.
"""
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from harness import manifest_insert  # noqa: E402

ROOT = Path(__file__).resolve().parents[1]
REPORTS = ROOT / 'build/harness/reports'


def rewrite_manifest(manifest, rows):
    """Write back one compact object per line, preserving the file format."""
    body = [json.dumps(r, separators=(',', ':')) for r in rows]
    manifest.write_text('[\n' + ',\n'.join(body) + '\n]\n')


def rewrite_globals(path, rows):
    body = [json.dumps(r, separators=(',', ':')) for r in rows]
    path.write_text('[\n' + ',\n'.join(body) + '\n]\n')


def apply_line(req):
    kind = req['kind']
    fn = req['fn']
    manifest = ROOT / 'config/functions.json'
    if kind == 'symbol':
        rows = json.loads(manifest.read_text())
        for row in rows:
            if row['id'] == fn:
                if row['symbol'] != req['symbol']:
                    row['symbol'] = req['symbol']
                    rewrite_manifest(manifest, rows)
                    return f'symbol {fn} -> {req["symbol"]}'
                return None  # already applied
        raise ValueError(f'{fn} not in manifest')
    if kind == 'flags':
        rows = json.loads(manifest.read_text())
        for row in rows:
            if row['id'] == fn:
                if row.get('extra_flags') != req['flags']:
                    row['extra_flags'] = req['flags']
                    rewrite_manifest(manifest, rows)
                    return f'flags {fn} -> {req["flags"]}'
                return None
        raise ValueError(f'{fn} not in manifest')
    if kind == 'global':
        path = ROOT / 'config/globals.json'
        rows = json.loads(path.read_text()) if path.exists() else []
        if any(int(r['address'], 16) == int(req['address'], 16) for r in rows):
            return None  # address already mapped
        rows.append({'symbol': req['symbol'], 'address': req['address']})
        rewrite_globals(path, rows)
        return f'global {req["symbol"]} = {req["address"]}'
    if kind == 'callee':
        entry = req['entry']
        rows = json.loads(manifest.read_text())
        existing = next((r for r in rows if r['id'] == entry['id']), None)
        if existing is not None:
            if existing.get('symbol') == entry.get('symbol') and existing.get('size') == entry.get('size'):
                return None
            existing['symbol'] = entry['symbol']
            if entry.get('size'):
                existing['size'] = entry['size']
            rewrite_manifest(manifest, rows)
            return f'callee symbol {entry["id"]} -> {entry["symbol"]}'
        line = json.dumps({**entry, 'source': None}, separators=(',', ':'))
        manifest.write_text(manifest_insert(manifest.read_text(), line))
        return f'callee entry {entry["id"]} ({entry.get("symbol")})'
    raise ValueError('unknown kind ' + kind)


def main():
    applied_log = REPORTS / 'applied.jsonl'
    results = []
    for queue in sorted(REPORTS.glob('cluster*.jsonl')):
        lines = [l for l in queue.read_text().splitlines() if l.strip()]
        kept = []
        for line in lines:
            try:
                note = apply_line(json.loads(line))
            except Exception as error:  # noqa: BLE001 - report and keep for retry
                results.append(f'ERROR {queue.name}: {line}: {error}')
                kept.append(line)
                continue
            if note:
                results.append(note)
                with applied_log.open('a') as stream:
                    stream.write(line + '\n')
        if kept or not lines:
            queue.write_text(''.join(l + '\n' for l in kept))
        else:
            queue.unlink()
    print('\n'.join(results) if results else 'nothing to apply')
    # anything left in queues needs attention
    leftover = [q.name for q in REPORTS.glob('cluster*.jsonl') if q.read_text().strip()]
    if leftover:
        print('RETRY NEEDED: ' + ', '.join(leftover))


if __name__ == '__main__':
    main()
