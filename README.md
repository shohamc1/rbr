# Richard Burns Rally reconstruction

Target: Windows x86, the **NoCD EXE** (assumed compatible with 1.0.2). The exact reference is pinned in `config/reference.json`.
Modern graphics, new platforms, and input changes are deferred.

This repository has a verified **function-matching baseline**, not a rebuilt game.
Three active C/C++ functions match 21 original bytes. The full executable and DLL
link remains blocked. `make check-integrated` reports that limit and fails.
`make check-overlay` is a separate intermediate check with original byte fallbacks.

## Start

On the tested Apple Silicon macOS host, Python 3.11.15, Clang 23.1.1,
Ghidra 12.1.4, bsdtar, and unshield 1.6.2 are available.
Install missing tools with `brew install llvm ghidra unshield openjdk@21`.
The compiler hash in `config/toolchain.json` must match; a newer package is not
silently accepted. This compiler is qualified only for the three matched leaves.

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
Clang remains the default for the three integrated leaves. The complete original
build options and full link layout remain unresolved. Windows gameplay and 1.0.2 compatibility have not been tested;
compatibility is assumed at the user's request.

`make check-function` compiles active source and compares its complete function
section with the pinned range. For an example, read
`src/reconstruction/sub_00401000.cpp` and its record in `analysis/functions/`.
`make check-overlay` replaces matching ranges in a reference copy and compares the
whole file without normalization. Retained bytes do not count as recovered source.

## Ghidra analysis for agents

`make ghidra-analyze` imports the pinned EXE, seeds the six reviewed function
boundaries, runs full analysis, and exports a function index and six function
packets. It takes about 90 seconds on the tested host. It replaces only its
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

For a compatible new source, set `"compiler": "msvc71"` in its function manifest;
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
