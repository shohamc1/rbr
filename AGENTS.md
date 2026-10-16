# Richard Burns Rally (Windows x86): matching decompilation

Goal: C/C++ that rebuilds the selected executable byte for byte, then a native
port built from that source. Modernization is deferred.

## The rule

The target is `original/nocd/richardburnsrally.exe`, pinned by SHA-256 in
`config/reference.json`. Assume compatibility with 1.0.2, as directed by the user. Use the base-install
HedgeHog3D and LocoMotion DLLs. Do not substitute the disc-root EXE or patch DLLs.
The user selected this reference explicitly; future target changes need an explicit
request. Never change a hash, range, compiler pin, or checker just to pass a test.

`make test check-overlay progress` must pass before integration or a commit.
Every claimed match must come from active compiled C/C++. Original bytes are not
recovered source. The overlay check retains original bytes and is only an
intermediate gate. `make check-integrated` must keep failing until a full source
rebuild exists. Do not weaken it or describe the overlay as a rebuilt game.

Never commit disc images, executables, DLLs, assets, extracted SDK headers,
compiler binaries, generated assembly, or original bytes copied into C.
Only source, configs, tools, tests, instructions, and text evidence belong in Git.
Keep disc/, original/, and build/ ignored.

## Read the evidence first

Before the first function, read README.md and config/toolchain.json. Before
choosing a target, read the initial queue below, its entry in
config/functions.json, and analysis/notes.md. Run `make ghidra-analyze` once, then
`make ghidra-function FUNCTION=<id>` for pseudocode, assembly, and references.
Run `make analyze` for the lightweight PE metadata records.
These files are the local knowledge base; do not import PSX toolchain assumptions.

Treat pseudocode, names, types, and compiler guesses as hypotheses. Keep supporting
module addresses and confidence with every recovered name or interface. Record
what you learn in the same session. Keep compact manual findings and open questions
in analysis/notes.md. Do not duplicate inventories, check logs, or generated code
in manual notes. Do not create batch or session reports. Generated analysis/functions/
files are ignored and replaceable; regenerate them with `make analyze`.
No external memory update is required.

## Layout and setup

| Path | Contents |
| --- | --- |
| disc/, original/ | Local media and extracted references; never tracked |
| src/reconstruction/ | Active recovered C/C++; one leaf per file for now |
| config/functions.json | Compact manifest: ID, size, symbol, compiler settings, and draft exceptions |
| config/reference.json | Selected executable and full paths to companion modules |
| config/toolchain.json | Qualified leaf compiler, library hashes, and flags |
| analysis/notes.md | Compact manual findings and unresolved work |
| analysis/functions/ | Ignored generated decoding and reference candidates |
| tools/, tests/ | Extraction, checks, analysis scripts, and tests |
| build/ | Generated objects, experiments, logs, and Ghidra projects |

Run `make setup`, then `make extract ISO='/absolute/path/to/disc.iso'` in a fresh
checkout. See README.md for tools. Run `make verify-inputs` and `make check-overlay`.
A worktree may link a verified original/ directory, but must have its own build/
and .venv/. Reserve src/modern/ and build/modern/ for the future port.

## The loop

1. Use a task branch or `git worktree add -b decomp/<packet> <path> HEAD`.
2. Claim one ready packet. Prefer a near-matching draft, then a small function with
   a matched analogue, then another bounded leaf. Respect the packet's ownership.
3. Export its Ghidra packet with `make ghidra-function FUNCTION=<id>`. Read the
   original body, callers, callees, and data references. Confirm boundaries;
   a RET is not the end if a branch reaches code after it.
4. Write the smallest readable C/C++ representation. Keep uncertain layouts local
   and assert offsets and sizes. Update the function's entry in config/functions.json;
   set `"nonmatching": true` until the check passes.
5. Run `make check-function FUNCTION=<id>`. It rebuilds active source every time,
   rejects stale objects and unresolved relocations, and prints exact differences.
6. On a match, remove `nonmatching`. Keep durable findings in analysis/notes.md and
   check output in build/. Run `make test check-overlay progress` before integration.

The current COFF checker accepts only one relocation-free function per object.
Coordinate linker/checker work before adding globals, several emitted functions,
RTTI, or exception tables. Do not hide those limitations with inline assembly,
binary includes, or conditional original-byte fallbacks. Review source as well as
checker output; the checker cannot prove that C++ contains no disguised assembly.

## When a function will not match

If eight compiles in a row do not reduce the difference, stop changing source.
Record the remaining size, byte, register, or instruction-order difference and
what you tried. Keep a readable draft marked `"nonmatching": true`, then choose
another ready packet. There is no configured RBR permuter; do not copy commands
from another game's instructions. Remove one-off probes from tracked paths.

Experiment with compiler options per function. Change shared defaults only after
a representative comparison improves results without losing any existing match.
Three tiny leaves do not qualify a compiler for the whole game. RTTI, exceptions,
custom register conventions, and floating-point state remain separate questions.

## Name things as evidence permits

Unknown functions use `sub_<8-digit lowercase VA>`; unknown fields use
`field_<hex offset>`. Always identify the module, since DLL bases can overlap.
Once purpose is supported, prefer descriptive PascalCase function/type names,
`gPascalCase` globals, and lowerCamelCase fields, locals, and parameters. Update
all callers and manifests with a rename, then rerun the checks. C++ symbol names
can change linkage; do not assume a rename is byte-neutral.

Use types and calling conventions supported by this x86 artifact. Do not impose
NASCAR's C89 or MIPS rules. The Clang leaf profile permits C++ layout assertions;
it does not prove that the original compiler accepted modern C++ syntax. Name
shared data only after its address and role are supported. Coordinate shared
headers instead of merging classes merely because their accessors are adjacent.

Write clear, idiomatic C++ supported by the selected compiler. Use classes,
methods, constructors, and other language features when evidence supports them.
Do not distort the source or invent class relationships solely to force a byte
match. If readable source does not match, keep it marked `"nonmatching": true` and
record the difference.

## Work together and integrate

Own only the claimed source and its manifest entry. Coordinate edits to shared
config/functions.json and analysis/notes.md; preserve other entries and findings.
When agents work in parallel, the integrator applies manifest and manual-note edits.
Coordinate changes to shared headers, tool scripts, Makefile, and reference/compiler
configuration with the integrator. Preserve others' edits; never revert unrelated work. Merge one
packet at a time, rerun the gate, and close its packet only after the merged source
passes. Track matching C/C++, non-matching C/C++, assembly, binary fallbacks, and
unresolved regions separately. Behavior tests add evidence; they do not replace
exact matching. State the tested artifact and limits in every progress update.

Use short declarative technical prose. Commit subjects use a capitalized imperative
of about 50 characters or fewer. Keep commits scoped. Do not push without a request.

Keep one compact JSON object per manifest line. The full executable hash in
config/reference.json pins all reference bytes; omit per-function hashes and status.
All source entries must match unless marked `"nonmatching": true`. Checks must fail
on a difference in any other source entry. Standard addresses and source paths are
derived from the ID; add `address` or `source` only for exceptions. Set `source` to
`null` for unresolved entries. Keep boundary notes in analysis/notes.md.

## Initial work queue

These packets target the pinned NoCD EXE. For each ID, own only
`src/reconstruction/<id>.cpp` and its entry in `config/functions.json`.
Coordinate shared manifest and manual-note edits with the integrator.

| Packet | Function | Size | Observed operation | Status |
| --- | --- | --- | --- | --- |
| P1 | sub_00401010 | 4 bytes | Read byte at ECX+4 into AL | Matched |
| P2 | sub_00406900 | 10 bytes | Write stack word to ECX+0x28; RET 4 | Matched |
| P3 | sub_00406940 | 10 bytes | Write stack word to ECX+0x74; RET 4 | Matched |

The open drafts are `sub_00408d00`, `sub_004183d0`, and `sub_0043ebb0`.
See analysis/notes.md for their remaining differences. Keep them separate from
matching progress.

Addresses are encoded in the IDs. Boundary confidence is high: each reviewed
body is straight-line code with an aligned entry, a final RET, and INT3 padding.
These bodies have no calls or global-data dependencies. The scan found no direct
call or data-pointer candidates; indirect callers and class identity remain
unknown. Fields may be flags, enums, integers, or pointers. Do not merge layouts
based on proximity. P1 leaves upper EAX bits unspecified.

Use a local out-of-line C++ member and an offset assertion. Follow the matched
accessor or setter example. Each packet is independent and needs no shared header.
Run `make check-function FUNCTION=<id>`, then `make test check-overlay progress`.
Completion requires an exact active-source match and updated manual notes and manifest entry.

MSVC 13.10.3077 is a tested candidate. Run `make setup-msvc qualify-msvc`.
It matches all five probes with `/arch:SSE` for the constructor. Set
`"compiler": "msvc71"` in a compatible config/functions.json entry to use it in the normal
checker; keep the shared Clang default until broader evidence supports a change.
Coordinate config/msvc71.json, tools/docker/msvc/Dockerfile, and tools/qualify_msvc.py.
Docker with Wine is the default MSVC runner. Start Docker before setup or checks.
`make setup-msvc` builds the pinned-base linux/386 image and saves its exact ID
under ignored tools/local/. Keep compiler binaries local; mount them at runtime.
The tested host is Apple Silicon with OrbStack x86 emulation. On another host,
run `make qualify-msvc check-msvc-link` before claiming runner support.
`make check-msvc-link` passes and verifies the linked probe's entry bytes. It does
not run the probe or prove the full game link. See README.md for image pin limits.

Ghidra exports and its function index are hypotheses, not matching progress.
Review boundaries, inferred conventions, and types against the instructions.
Generated projects and pseudocode stay in build/; never commit them. Re-running
`make ghidra-analyze` replaces its generated project and exports. Keep manual
Ghidra work elsewhere. Runtime behavior and the full source link remain unproved.
