# Richard Burns Rally reconstruction

Target: Windows x86, the **NoCD EXE** (assumed compatible with 1.0.2). The exact reference is pinned in `config/reference.json`.
Modern graphics, new platforms, and input changes are deferred.

This repository has a verified **function-matching baseline**, not a rebuilt game.
103 active C/C++ functions match 1,694 original bytes. Three more functions remain
nonmatching C++ drafts. The matches include a 549-byte array routine.
The full executable and DLL link remains blocked.
`make check-integrated` reports that limit and fails.
`make check-overlay` is a separate intermediate check with original byte fallbacks.

## Start

On the tested Apple Silicon macOS host, Python 3.11.15, Clang 23.1.1,
Ghidra 12.1.4, bsdtar, and unshield 1.6.2 are available.
Install missing tools with `brew install llvm ghidra unshield openjdk@21`.
The compiler hash in `config/toolchain.json` must match; a newer package is not
silently accepted. Compiler results apply only to the listed exact matches;
the general game profile remains unresolved.

```sh
make setup
# In a fresh checkout only. Supply the local ISO; it is not in Git.
make extract ISO='/absolute/path/to/Richard Burns Rally [149731fe].iso'
make verify-inputs
make build
make check-function FUNCTION=sub_00401000
make check-overlay
make test
make qualify # expected failure: two candidate functions do not match
make progress
make analyze
make ghidra-analyze
make ghidra-function FUNCTION=sub_00401000
```

Extraction needs about 5 GB plus the ISO. It refuses to replace `original/`.
An existing, hash-verified `original/` can be shared by worktrees with a symlink.
Each worktree must have its own `.venv/` and `build/`.

Read [AGENTS.md](AGENTS.md) for the matching loop and initial work queue.

## Reference and limits

Extraction creates `original/nocd/richardburnsrally.exe` as a byte-identical copy
of the ISO member `Crack/richardburnsrally.exe`. The base-install HedgeHog3D and
LocoMotion DLLs are companion targets. Patch files remain comparison evidence.
`config/artifacts.json` records all extracted hashes, including the NoCD alias.
No game payload is tracked.

MSVC 13.10.3077 is now a tested candidate: it matches all five probes.
Clang remains the default for compatible leaves. The complete original
build options and full link layout remain unresolved. Windows gameplay and 1.0.2 compatibility have not been tested;
compatibility is assumed at the user's request.

`make check-function` compiles active source and compares its complete function
section with the pinned range. For an example, read
`src/reconstruction/sub_00401000.cpp` and its entry in `config/functions.json`.
`make check-overlay` replaces matching ranges in a reference copy and compares the
whole file without normalization. Retained bytes do not count as recovered source.

`config/functions.json` stores one compact JSON object per line. Each entry keeps
its ID, size, symbol, and any compiler settings. The full executable SHA-256
in `config/reference.json` pins the reference bytes; per-function hashes are redundant.
The loader derives the address from `sub_<VA>` and the source path from
`src/reconstruction/<id>.cpp`. Set `address` or `source` only for exceptions;
set `source` to `null` for an unresolved entry. All source entries must match
unless marked `"nonmatching": true`. Remove that flag after an exact check passes.
Overlay and progress checks rebuild all expected matches and fail on a difference.
`make progress` then prints the saved boundary census by core, runtime/crash, and
auxiliary scope. It shows candidate entries, code-section bytes, unresolved regions,
and generated pseudocode counts. `make census-progress` shows this summary without
compiling source or running Ghidra. Verified C/C++ function and byte counts remain
separate from analysis candidates. No pseudocode export counts as a source match.

The summary checks snapshot tool/configuration hashes and module pins. It rejects
changed local boundary evidence or pseudocode files. Missing ignored evidence uses
the saved snapshot with an explicit notice. The pinned module files are still
required. An unanalyzed module has an unknown
function count; its code bytes stay in the total. After analysis or export changes,
run `make census` to refresh `analysis/function-census.json` and
`analysis/module-scope.json`. This refresh needs the local census evidence.
Keep manual findings and unresolved questions in [analysis/notes.md](analysis/notes.md).
`make analyze` regenerates the ignored `analysis/functions/` files from the pinned
reference and manifest. Checks do not require these files. Keep generated assembly,
pseudocode, references, and logs local. Do not create batch or session reports.

## Ghidra analysis for agents

`make ghidra-analyze` imports the pinned EXE, seeds the function boundaries in
`config/functions.json`, runs full analysis, and exports an index and a packet for
each manifest entry. The run time depends on the manifest. It replaces only its
generated project and exports under `build/ghidra/<reference-hash-prefix>/`.
Do not store manual work in this project; use a separate Ghidra project for that.
The old `sh tools/ghidra-import.sh` command runs this same workflow.

`make ghidra-function FUNCTION=004010e0` exports one function from the saved project
without repeating analysis. IDs such as `sub_00401000` also work. Each JSON packet
contains assembly, C-like pseudocode, body ranges, inferred prototype, and incoming
and outgoing references, including imported symbols and referenced data.
`exports/functions.json` is the candidate inventory; `exports/complete.json`
records the Ghidra version, reference hash, and all analysis options. Logs remain
beside the exports. All these generated files are ignored by Git.

Ghidra 12.1.4 and analyzer enablement are pinned in `config/ghidra.json`. Other
options use that version's defaults and are recorded in the export receipt. The
language is x86 little-endian 32-bit, with the Windows compiler specification.
Set `GHIDRA_HEADLESS` or `JAVA_HOME` to select installed tool paths. Timeouts,
missing functions, failed decompilation, wrong references, and missing completion
receipts cause failure; old exports cannot satisfy the selected function check.

The tested analysis found 15,874 function candidates. This is not a recovered-source
count. Boundaries, types, and calling conventions need review. Ghidra calls the
0x00401000 getter `fastcall`; matching source uses a C++ `thiscall` representation.
Header data references are not proof of callers. The analyzer also reported invalid
PNG resource candidates. Review these warnings instead of treating inferred data
as fact. `make analyze` remains the separate, lightweight PE/Capstone metadata scan.

## Function census and DLL scope

The original 15,874-entry EXE index is incomplete. Use the separate census
projects for counts and boundary review. See [analysis/notes.md](analysis/notes.md) for reviewed findings.
`analysis/function-census.json` and `analysis/module-scope.json` hold current measurements.

```sh
make census-inventory # hash-checked base-install PE inventory
make census-analyze   # independent, unseeded projects for game and support modules
make census-refine    # virtual tables, bounded branch paths, and reviewed corrections
make census-audit     # refresh evidence from saved projects without changes
make census-decompile # export all candidates in DLLs with supported load paths
make census           # refresh saved reports from local census evidence
make census-progress  # quick saved summary; no compile or Ghidra run
```

Run another refinement pass when the report still has new supported entry
candidates. The tool retains the baseline for comparison. Projects, instruction
evidence, pseudocode, and per-address queues stay under ignored `build/census/`.
It never changes the matching-work Ghidra project or function manifests.

`config/census.json` records reviewed DLL load paths and boundary corrections.
The inventory follows normal and delay imports. A DLL name in a string is only
a candidate. Runtime-selected modules, launcher dependencies, unused bundled
files, and text files with a `.dll` suffix have separate records.

Counts describe distinct machine-code entry candidates in each module. They
include thunks and generated library routines. They do not count inlined source
functions, identify every original source function, or prove a complete count.
All code bytes remain in the size denominator, including unexplained ranges.
No census entry counts as recovered C/C++. Pseudocode exports retain per-function
failures and must agree with the current module hash, entry list, and body sizes.
They remain unreviewed analysis drafts, even when Ghidra reports success.

## MSVC candidate

```sh
make setup-msvc
make qualify-msvc
make check-msvc-link
```

Setup downloads the package listed by [decomp.me's compiler catalog](https://github.com/decompme/compilers/blob/4694562e964e6198f7fef863ed48462bf1ff2f49/values.yaml)
into ignored `tools/local/` and builds the default Docker Wine runner. Start a
Docker engine before setup or any MSVC command. The runner uses 32-bit Debian
Linux (`linux/386`) and Wine 8.0. Apple Silicon needs working x86 emulation;
this setup is tested with OrbStack. Other Docker hosts need the same checks.

The compiler archive and files are hash-pinned in `config/msvc71.json`.
`tools/docker/msvc/Dockerfile` pins the Debian base digest and Wine package version.
APT resolves supporting packages at image build time; this is not a complete
package snapshot. Setup records the built image ID in ignored
`tools/local/msvc-wine-image.txt`; compiler calls use that ID, not a mutable tag.
Run setup again if the image is removed. Keep the compiler local; it is mounted
at runtime and is not included in the image. Compiler and linker calls have no
network access. Source and compiler mounts are read-only; only the output
directory is writable. Each call uses a fresh Wine prefix.

The package filename says `msvc7.0`, but its actual compiler is **13.10.3077**
and linker is **7.10.3077**. This is a community compiler package, not a newly
authenticated Microsoft distribution.

The disc-root EXE and base DLL Rich metadata contain product/build 0x60/3077,
which also appears in a candidate-compiled C++ object. The NoCD EXE has no Rich
header. Together with five exact code matches, this supports MSVC 7.1 build 3077
as a strong candidate; it does not establish every original module or build option.
See `analysis/compiler-evidence.json` for address-independent artifact evidence.

The tested flags are `/O2 /Zl`, with `/arch:SSE` for the two-float constructor.
All five probes match: getter, setter, x87 absolute value, predicate, and constructor.
The older compiler uses equivalent typedef assertions and a `fabs` intrinsic in
its probe sources instead of Clang-specific syntax. These are qualification probes,
not additional recovered production functions. Reports go to
`build/compiler/qualification.json`.

For a compatible new source, set `"compiler": "msvc71"` in its `config/functions.json` entry;
use `"extra_flags": ["/arch:SSE"]` only when evidence supports it. The existing
`make check-function` and overlay gates then use that compiler. Omitted compiler
fields keep Clang. Do not pass Clang options to MSVC or change the shared default
from five small samples.

`make check-msvc-link` compiles and links a small probe through Docker Wine.
It checks that the output is an i386 PE and that its entry bytes equal the
compiled function. This passes; the former wibo `lstrcmpiA` blocker is resolved.
The probe is not run. The game's complete source and layout remain blockers to
a full rebuild. Virtual dispatch, exceptions, RTTI, custom register conventions,
and Windows floating-point state are not qualified.
