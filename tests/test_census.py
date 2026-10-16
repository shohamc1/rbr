from pathlib import Path
from contextlib import redirect_stdout
from io import StringIO
import json
import struct
import sys
import unittest
import tempfile
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from boundaries import find_vtables, leaf_size, bounded_cfg, discovery
from census import closure, imports, module_scope, summarize, validate, validate_decisions, validate_decompilation, validate_pseudocode
from types import SimpleNamespace
import census


class CensusTests(unittest.TestCase):
    def test_progress_uses_saved_counts_and_rejects_stale_local_evidence(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            def put(name, value):
                path = root / name
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text(json.dumps(value))
                return path
            tool = put('tools/census.py', 'tool')
            put('config/reference.json', {'reconstruct': ['game.exe', 'engine.dll']})
            put('config/census.json', {'game_root': 'game.exe', 'auxiliary_roots': [],
                                     'dynamic_loads': [{'from': 'game.exe', 'to': 'engine.dll'}],
                                     'missing_optional_modules': [{'name': 'optional.dll'}]})
            modules = [{'path': 'game.exe', 'sha256': 'game', 'code_bytes': 100,
                        'imports': [{'dll': 'runtime.dll'}]},
                       {'path': 'engine.dll', 'sha256': 'engine', 'code_bytes': 50, 'imports': []},
                       {'path': 'runtime.dll', 'sha256': 'runtime', 'code_bytes': 30, 'imports': []},
                       {'path': 'unused.dll', 'sha256': 'unused', 'code_bytes': 7, 'imports': []}]
            rows = []
            for module, count in zip(modules, [5, 3, 4]):
                rows.append({'path': module['path'], 'sha256': module['sha256'],
                             'pe_virtual_code_bytes': module['code_bytes'], 'function_candidates': count,
                             'evidence': 'build/' + module['path'] + '/boundaries.json',
                             'evidence_sha256': census.digest(tool), 'pseudocode_export': {'status': 'not_exported'},
                             'unowned_instruction_bytes': 10, 'undefined_bytes_by_kind': {'unresolved': 20},
                             'issues_by_kind': {'unresolved_indirect_jump': 2}})
            pseudocode = put('build/engine/decompiled/pseudocode.jsonl', 'pseudocode')
            rows[1]['pseudocode_export'] = {'status': 'current_entries_and_sizes', 'completed': 3,
                                            'failed_entries': [], 'path': 'build/engine/decompiled',
                                            'pseudocode_sha256': census.digest(pseudocode)}
            snapshot = {'generated_at_utc': '2026-10-16T00:00:00+00:00', 'modules': rows,
                        'tool_sha256': {'tools/census.py': census.digest(tool)}}
            snapshot_path = put('analysis/function-census.json', snapshot)
            def output():
                stream = StringIO()
                with redirect_stdout(stream):
                    census.progress()
                return stream.getvalue()
            with patch.object(census, 'ROOT', root), patch.object(census, 'inventory', return_value=modules):
                text = output()
                self.assertIn('Core game: 8 candidate entries; 150 code-section bytes', text)
                self.assertIn('Runtime/crash support: 4 candidate entries; 30 code-section bytes', text)
                self.assertIn('showing saved counts', text)
                self.assertIn('Pseudocode snapshot: 3 completed; 0 failed; 1 modules.', text)
                self.assertIn('No source-match credit', text)
                pseudocode.write_text('changed export')
                with self.assertRaisesRegex(ValueError, 'Pseudocode export changed'):
                    output()
                pseudocode.unlink()
                self.assertIn('showing saved export counts', output())
                self.assertIn('Game plus runtime/crash: 12 candidate entries; 180 code-section bytes', text)
                self.assertIn('optional.dll', text)
                # An absent module is unknown, and its bytes still count.
                snapshot['modules'] = rows[:1] + rows[2:]
                snapshot_path.write_text(json.dumps(snapshot))
                self.assertIn('Core game: at least 5 candidate entries; 150 code-section bytes', output())
                self.assertIn('Not analyzed: engine.dll', output())
                snapshot['modules'] = rows
                snapshot_path.write_text(json.dumps(snapshot))
                evidence = put(rows[0]['evidence'], 'changed')
                with self.assertRaisesRegex(ValueError, 'evidence changed'):
                    output()
                evidence.unlink()
                put(str(evidence.parent.relative_to(root) / 'baseline.json'), {})
                with self.assertRaisesRegex(ValueError, 'analysis incomplete'):
                    output()
                tool.write_text('changed tool')
                with self.assertRaisesRegex(ValueError, 'snapshot is stale'):
                    output()
                snapshot_path.unlink()
                with self.assertRaisesRegex(ValueError, 'snapshot unavailable'):
                    output()

    def test_failed_report_removes_stale_outputs(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            (root / 'analysis').mkdir()
            outputs = [root / 'analysis' / n for n in ['function-census.json', 'module-scope.json']]
            for output in outputs:
                output.write_text('stale success')
            with patch.object(census, 'ROOT', root), patch.object(census, 'inventory', side_effect=ValueError('hash mismatch')):
                with self.assertRaisesRegex(ValueError, 'hash mismatch'):
                    census.report()
            self.assertTrue(all(not p.exists() for p in outputs))

    def test_leaf_requires_straight_line_code_and_complete_padding(self):
        self.assertEqual(leaf_size(bytes.fromhex('8a 41 04 c3') + b'\xcc' * 12, 0x401000), 4)
        self.assertIsNone(leaf_size(bytes.fromhex('eb 02 c3 cc c3') + b'\xcc' * 11, 0x401000))
        self.assertIsNone(leaf_size(bytes.fromhex('e8 00 00 00 00 c3') + b'\xcc' * 10, 0x401000))
        self.assertIsNone(leaf_size(bytes.fromhex('8a 41 04 c3'), 0x401000))
        self.assertIsNone(leaf_size(bytes.fromhex('fa c3') + b'\xcc' * 14, 0x401000))

    def test_cfg_follows_branches_after_return_and_rejects_hidden_bytes(self):
        # TEST EAX,EAX; JE second return; RET; XOR EAX,EAX; RET.
        code = bytes.fromhex('85 c0 74 01 c3 33 c0 c3') + b'\xcc' * 8
        self.assertEqual(bounded_cfg(code, 0x401000, set())['size'], 8)
        self.assertIsNone(bounded_cfg(bytes.fromhex('eb 01 90 c3') + b'\xcc' * 12, 0x401000, set()))
        self.assertIsNone(bounded_cfg(bytes.fromhex('ff e0') + b'\xcc' * 14, 0x401000, set()))
        self.assertIsNone(bounded_cfg(bytes.fromhex('eb ff') + b'\xcc' * 14, 0x401000, set()))
        aligned = b'\x90' * 15 + b'\xc3'
        self.assertEqual(bounded_cfg(aligned, 0x401000, set())['size'], 16)
        call = bytes.fromhex('e8 fb 0f 00 00 c3') + b'\xcc' * 10
        self.assertIsNone(bounded_cfg(call, 0x401000, set()))
        self.assertEqual(bounded_cfg(call, 0x401000, {0x402000})['size'], 6)

    def test_discovery_accepts_an_entry_after_a_decoded_aligned_return(self):
        base = 0x401000
        image = (b'\x90' * 15 + b'\xc3' + b'\x8b\xc0' + b'\x90' * 13 + b'\xc3' +
                 bytes.fromhex('8a 41 04 c3') + b'\xcc' * 12)
        pe = SimpleNamespace(OPTIONAL_HEADER=SimpleNamespace(ImageBase=base),
                             sections=[SimpleNamespace(VirtualAddress=0, Misc_VirtualSize=48,
                                                       SizeOfRawData=48, Characteristics=0x20000000)],
                             get_memory_mapped_image=lambda: image)
        report = {'sha256': 'hash', 'functions': [{'address': '00401000', 'ranges': [
            {'start': '00401000', 'end_inclusive': '0040100f'}]}],
            'aligned_return_successors': ['00401010']}
        result = discovery(pe, report)
        self.assertEqual(result['return_entry_candidates'], ['00401010', '00401020'])
        self.assertEqual(result['padding_entry_candidates'], [])
        self.assertEqual([c['size'] for c in result['bounded_cfg_seeds']], [16, 4])
        report['aligned_return_successors'] = []
        self.assertEqual(discovery(pe, report)['bounded_cfg_seeds'], [])

    def test_import_closure_is_case_insensitive_and_handles_cycles(self):
        edges = {'game.exe': ['Engine.DLL', 'KERNEL32.dll'], 'engine.dll': ['game.exe']}
        self.assertEqual(closure(['GAME.EXE'], edges), {'game.exe', 'engine.dll', 'kernel32.dll'})

    def test_delay_imports_are_not_lost(self):
        item = SimpleNamespace(dll=b'Optional.dll', imports=[1, 2])
        self.assertEqual(imports(SimpleNamespace(DIRECTORY_ENTRY_DELAY_IMPORT=[item])),
                         [{'dll': 'Optional.dll', 'kind': 'delay_import', 'symbols': 2}])

    def test_scope_keeps_auxiliary_and_unproved_modules_separate(self):
        modules = [{'path': 'game.exe', 'imports': [{'dll': 'CRT.dll'}]},
                   {'path': 'Engine.dll', 'imports': []},
                   {'path': 'CRT.dll', 'imports': []},
                   {'path': 'Launcher.exe', 'imports': [{'dll': 'Unicode.dll'}]},
                   {'path': 'Unicode.dll', 'imports': []},
                   {'path': 'Unused.dll', 'imports': []}]
        cfg = {'game_root': 'game.exe', 'auxiliary_roots': ['Launcher.exe'],
               'dynamic_loads': [{'from': 'game.exe', 'to': 'Engine.dll'}],
               'missing_optional_modules': []}
        scope = module_scope(modules, cfg)
        self.assertEqual(scope['game_local_modules'], ['CRT.dll', 'Engine.dll', 'game.exe'])
        self.assertEqual(scope['auxiliary_local_modules'], ['Launcher.exe', 'Unicode.dll'])
        self.assertEqual(scope['local_modules_without_load_edge'], ['Unused.dll'])

    def test_rtti_table_needs_valid_locator_type_and_hierarchy(self):
        base = 0x400000
        image = bytearray(0x200)
        def put(offset, *values):
            struct.pack_into('<' + 'I' * len(values), image, offset, *values)
        put(0x100, base + 0x120, base + 0x20, base + 0x40, 0)
        put(0x120, 0, 0, 0, base + 0x160, base + 0x180)
        image[0x168:0x174] = b'.?AVExample\0'
        put(0x180, 0, 0, 1, base + 0x1a0)
        data = [(base + 0x100, base + 0x200)]
        code = [(base, base + 0x80)]
        tables = find_vtables(image, base, data, code)
        self.assertEqual(len(tables), 1)
        self.assertEqual(tables[0]['targets'], ['00400020', '00400040'])
        put(0x120, 1)
        self.assertEqual(find_vtables(image, base, data, code), [])
        put(0x120, 0)
        put(0x18c, base + 0x300)
        self.assertEqual(find_vtables(image, base, data, code), [])

    def test_vtable_stops_before_text_that_looks_like_a_code_address(self):
        base = 0x400000
        image = bytearray(0x200)
        def put(offset, *values):
            struct.pack_into('<' + 'I' * len(values), image, offset, *values)
        put(0x100, base + 0x120, base + 0x20, 0x00534f50, 0)
        put(0x120, 0, 0, 0, base + 0x160, base + 0x180)
        image[0x168:0x174] = b'.?AVExample\0'
        put(0x180, 0, 0, 1, base + 0x1a0)
        # The second apparent pointer is the text label POS followed by NUL.
        result = find_vtables(image, base, [(base + 0x100, base + 0x200)],
                              [(base, base + 0x80), (0x00534f50, 0x00534f60)],
                              [(base + 0x108, base + 0x10c)])
        self.assertEqual(result[0]['targets'], ['00400020'])

    def test_decompilation_receipt_requires_current_entries_and_counts(self):
        audit = {'sha256': 'hash', 'ghidra_version': '1', 'functions': [{'address': '00401000'}]}
        receipt = {'sha256': 'hash', 'ghidra_version': '1', 'entries': ['00401000'],
                   'completed': 1, 'failed_entries': []}
        validate_decompilation(audit, receipt)
        with self.assertRaises(ValueError):
            validate_decompilation(audit, {**receipt, 'entries': ['00401010']})
        with self.assertRaises(ValueError):
            validate_decompilation(audit, {**receipt, 'completed': 0})

    def test_pseudocode_export_checks_each_body_and_completion(self):
        audit = {'functions': [{'address': '00401000', 'size': 4}]}
        with tempfile.TemporaryDirectory() as temp:
            path = Path(temp) / 'pseudocode.jsonl'
            path.write_text('{"address":"00401000","size":4,"completed":true,"pseudocode":"return;"}\n')
            validate_pseudocode(audit, {'failed_entries': []}, path)
            audit['functions'][0]['size'] = 5
            with self.assertRaisesRegex(ValueError, 'body differs'):
                validate_pseudocode(audit, {'failed_entries': []}, path)
            path.write_text('')
            with self.assertRaisesRegex(ValueError, 'Truncated'):
                validate_pseudocode(audit, {'failed_entries': []}, path)

    def test_summary_requires_complete_code_accounting(self):
        report = {'functions': [], 'undefined_ranges': [{'size': 3, 'kind': 'unresolved'}],
                  'function_body_union_bytes': 0, 'executable_bytes': 6,
                  'unowned_instruction_ranges': [{'size': 2}],
                  'data_outside_bodies_bytes': 1, 'issues': [], 'loader_functions': []}
        self.assertEqual(summarize(report)['undefined_bytes_by_kind'], {'unresolved': 3})
        self.assertFalse(summarize(report)['exact_count_proved'])
        report['executable_bytes'] += 1
        with self.assertRaisesRegex(ValueError, 'does not balance'):
            summarize(report)

    def test_export_rejects_wrong_artifact_version_and_options(self):
        settings = {'version': '1', 'analyzers': {'A': 'true'}}
        report = {'sha256': 'hash', 'ghidra_version': '1', 'analysis_options': {'A': 'true'}}
        validate(report, 'hash', settings)
        for key, value in [('sha256', 'other'), ('ghidra_version', '2'), ('analysis_options', {})]:
            with self.subTest(key=key), self.assertRaises(ValueError):
                validate({**report, key: value}, 'hash', settings)

    def test_report_rejects_a_lost_reviewed_return_contract(self):
        cfg = {'returning_imports': [{'symbol': 'delete_symbol', 'library': 'CRT.dll'}],
               'fragment_merges': [], 'code_data_corrections': []}
        external = {'name': 'CRT.DLL::delete', 'original_import': 'delete_symbol', 'no_return': True}
        report = {'functions': [], 'external_functions': [external]}
        with self.assertRaisesRegex(ValueError, 'still marked no-return'):
            validate_decisions(report, cfg, Path('game.exe'))
        external['no_return'] = False
        validate_decisions(report, cfg, Path('game.exe'))
