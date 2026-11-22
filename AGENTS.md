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

Before the first function, read README.md and config/toolchain.json, then run
`make doctor`. Before choosing a target, read analysis/notes.md and run
`make candidates`. Run `make ghidra-analyze` once, then
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
| src/reconstruction/ | Active recovered C/C++; one file can hold a class or related functions |
| config/functions.json | Compact manifest: ID, size, symbol, compiler settings, and draft exceptions |
| config/reference.json | Selected executable and full paths to companion modules |
| config/toolchain.json | Qualified leaf compiler, library hashes, and flags |
| analysis/notes.md | Compact manual findings and unresolved work |
| analysis/functions/ | Ignored generated decoding and reference candidates |
| tools/, tests/ | Extraction, checks, analysis scripts, and tests |
| build/ | Generated objects, experiments, logs, and Ghidra projects |

Run `make setup`, then `make extract ISO='/absolute/path/to/disc.iso'` in a fresh
checkout. See README.md for tools. Run `make verify-inputs` and `make check-overlay`.
Extraction needs about 5 GB plus the ISO and refuses to replace original/. It copies
Crack/richardburnsrally.exe to original/nocd/richardburnsrally.exe without changes.
config/artifacts.json records the hashes of extracted PE files and census
archives, including this alias. Game assets aren't pinned.
A worktree may link a verified original/ directory and tools/local/, but must have
its own build/ and .venv/. `make worktree` sets this up. Reserve src/modern/ and build/modern/ for the future port.

## The loop

The harness commands in `tools/harness.py` cover each step. Each command prints
the next command, so you can follow the output without holding the whole loop in
mind.

1. Check the environment: `make doctor`. Fix every `FAIL` line before you start.
   It checks the reference hash, the Clang and Ghidra pins, Docker, the MSVC image,
   stale MSVC containers, and the saved Ghidra and census evidence.
2. Pick a target: `make candidates`. Prefer a near-matching draft from
   analysis/notes.md, then the top unblocked candidate.
3. Claim it: `make worktree FUNCTION=<id>` creates `../rbr-<id>` on branch
   `decomp/<id>`. The branch name is the claim; `make candidates` hides it.
4. In the worktree, run `make new-function FUNCTION=<id>`. It checks the boundary,
   writes a scaffold source, adds a `"nonmatching": true` manifest entry, lists
   the calls, imports, globals, and blockers to handle, and exports the Ghidra
   packet. If the function already has a `"source": null` entry, it replaces it.
5. Read the original body, callers, callees, and data references. Confirm
   boundaries; a RET is not the end if a branch reaches code after it.
   `make lookup ADDR=0x<address>` names an import slot, global, manifest function,
   boundary candidate, section, or string.
6. Write the smallest readable C/C++ representation. Keep uncertain layouts local
   and assert offsets and sizes. The scaffold's calling convention is a guess from
   register use; correct it from the evidence.
7. Run `make check-function FUNCTION=<id>`. It rebuilds the active source every
   time, then prints a side-by-side listing, a score, an attempt count, and a hint.
8. On a match, remove `nonmatching`. Keep durable findings in analysis/notes.md.
   Run `make test check-overlay progress` before integration.

### Read check-function output

A mismatch prints the reference and compiled instructions side by side. Lines
marked `|` differ. The score is the number of differing instructions; 0 is a match.

The checker logs one attempt per distinct source text and flag set in
`build/harness/attempts/<id>.jsonl`. It prints the best score so far and the
number of compiles since that best. At eight compiles without improvement, it
prints `Stop`; follow "When a function will not match" below.

Every failure ends with a `hint:` line that names the next action:

- An unresolved call or data symbol prints the manifest or globals.json line to add.
- A symbol mismatch prints the symbol the object defines.
- Extra functions or data name the usual causes, such as destructors and literals.

### Read candidates output

`make candidates` decodes each unclaimed single-range boundary candidate up to
`MAX_SIZE` bytes (default 128) and ranks it. Use `LIMIT=<count>` to show more and
`TIER=<tier>` to filter. The full list is in `build/harness/candidates.json`.
Lower scores are easier: size plus weights for unknown callees, globals, imports,
and indirect calls. Entries already in the manifest with `"source": null` come
first. The tiers are:

`leaf`
: No calls, imports, or globals.

`relocations`
: Calls only manifest functions and imports, or reads globals. Map each in
  config/functions.json or config/globals.json.

`needs-callee-entries`
: Calls a function outside the manifest. Add a `"source": null` entry for it;
  the checker prints the line after the first compile.

`blocked`
: Uses a form the checker can't verify yet: SEH or C++ EH (`fs:`), a switch table,
  a special instruction such as CPUID, an unverified boundary, or a register input
  other than ECX and EDX. Hidden unless you pass `--all` to the script.

The notes column flags `ecx input: thiscall?`, `ecx/edx input: fastcall?`,
`ret N` stack cleanup, `/GS`, x87, and SSE. These flags come from a linear scan,
not data-flow analysis. Confirm them in the assembly.

One source file can define many functions. Each manifest entry whose functions
share a file sets `"source"` to that path. MSVC `/O2` implies `/Gy`, and the Clang
profile passes `-ffunction-sections`, so each function gets its own COMDAT
section. The checker reads only the named symbol's section and its relocations.
It compiles each source once per run. A call to another function defined in the
same object resolves by name, like an external. That function must be in the
manifest, and its own entry checks its bytes. Two functions in one section are
rejected. REL32 external relocations resolve
against manifest functions and IAT thunks. DIR32 externals resolve against PE
import slots, globals in config/globals.json, and manifest functions used as
data, such as callbacks. Both retain relocation addends. Compiler literals
(`__real@` floats and doubles, `??_C@` strings) take the address the reference
operand names, but only when read-only reference data holds the same bytes there;
one literal name keeps one address per run. Other object-local data, RTTI, and
SEH/C++-EH scope tables and funclet sections still need linker-aware checking. Do not hide
those limitations with inline assembly, binary includes, or conditional
original-byte fallbacks. Review source as well as checker output; the checker
cannot prove that C++ contains no disguised assembly.

## Source rules

`make test` enforces the first three rules in `tests/test_source_rules.py`.

- Never write a code or data address as an integer, such as
  `*(float*)0x00776a70` or `field_00 = 0x70c79c`. The checker accepts the
  immediate, but the source hides its target and breaks under any other layout.
  Write the literal (`0.0f`, `"rb"`), declare a named `extern "C"` global and map
  it in config/globals.json, or declare the function and give it a manifest
  entry. Vtables use `gVtable<address>`. Unknown data uses `g<Role><address>`.
- Omit empty optional manifest fields, such as `"extra_flags":[]`.
- Keep evidence in tracked files. Sources and analysis/notes.md must not cite
  ignored reports under build/; copy the finding into analysis/notes.md.
- A nonmatching draft must compute what the reference computes. Never use a
  placeholder body, such as returning the argument, to make a residual visible.
  Try the compiler intrinsic before you declare a form impossible: `/O2` implies
  `/Oi`, and `atan` expands inline to `FPATAN`.
- Remove comments that say a manifest entry is pending once it exists.

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

## Choose work

Use `make candidates` for new targets and analysis/notes.md for open drafts. For
each ID, own its source file and its entry in `config/functions.json`. To add a
function to an existing class file, run
`make new-function FUNCTION=<id> SOURCE=src/reconstruction/<file>.cpp`.
Claim a shared file before you edit it, and keep shared types in one definition. Coordinate shared manifest and manual-note edits with
the integrator. The manifest marks all open drafts with `"nonmatching": true`;
keep them separate from matching progress.

Boundary candidates are hypotheses. Fields may be flags, enums, integers, or
pointers. Don't merge layouts based on proximity. For accessors and setters, use
a local out-of-line C++ member and an offset assertion, following a matched
example such as `sub_00406900`.

## Ghidra analysis

Analysis requires the Ghidra version pinned in config/ghidra.json (12.2) and a
JDK that meets that release's `application.java.min`. The tools read that minimum
from the installed Ghidra and select the oldest Homebrew `openjdk@N` that meets
it, unless you set `JAVA_HOME`. A different Ghidra release fails verification.
Homebrew upgrades replace the pinned release; `make doctor` reports this drift.

`make ghidra-analyze` imports the pinned EXE, seeds the function boundaries in
`config/functions.json`, runs full analysis, and exports an index and a packet for
each manifest entry. The run time depends on the manifest. It replaces only its
generated project and exports under `build/ghidra/<reference-hash-prefix>/`.
Do not store manual work in this project; use a separate Ghidra project for that.
The old `sh tools/ghidra-import.sh` command runs this same workflow.

`make ghidra-function FUNCTION=004010e0` exports one function from the saved project
without repeating analysis. IDs such as `sub_00401000` also work. A worktree
without its own project reads the main checkout's saved project and writes exports
locally. The matching project seeds only manifest entries. If it has no function
at the address, the export retries against the census project. That retry warns
about analyzer differences instead of failing; treat its packet as boundary
evidence only. Each JSON packet
contains assembly, C-like pseudocode, body ranges, inferred prototype, and incoming
and outgoing references, including imported symbols and referenced data.
`exports/functions.json` is the candidate inventory; `exports/complete.json`
records the Ghidra version, reference hash, and all analysis options. Logs remain
beside the exports. All these generated files are ignored by Git.

Ghidra 12.2 and analyzer enablement are pinned in `config/ghidra.json`. Other
options use that version's defaults and are recorded in the export receipt. The
language is x86 little-endian 32-bit, with the Windows compiler specification.
Set `GHIDRA_HEADLESS` or `JAVA_HOME` to select installed tool paths. Timeouts,
missing functions, failed decompilation, wrong references, and missing completion
receipts cause failure; old exports cannot satisfy the selected function check.

The tested 12.2 analysis found 15,706 function candidates. This is not a recovered-source
count. Boundaries, types, and calling conventions need review. Ghidra calls the
0x00401000 getter `fastcall`; matching source uses a C++ `thiscall` representation.
Header data references are not proof of callers. The analyzer also reported invalid
PNG resource candidates. Review these warnings instead of treating inferred data
as fact. `make analyze` remains the separate, lightweight PE/Capstone metadata scan.

## Function census and DLL scope

The 15,706-entry matching-project EXE index is incomplete. Use the separate census
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

`make progress` first rebuilds all expected source matches, then prints the saved
boundary census by core, runtime/crash, and auxiliary scope. `make census-progress`
prints the saved summary without compiling source or running Ghidra. Keep verified
C/C++ counts separate from candidates and generated pseudocode.

The summary checks snapshot tool/configuration hashes and module pins. It rejects
changed local boundary evidence or pseudocode files. Missing ignored evidence uses
the saved snapshot with an explicit notice. The pinned module files are still
required. An unanalyzed module has an unknown function count; its code bytes stay
in the total. After analysis or export changes, run `make census` to refresh
`analysis/function-census.json` and `analysis/module-scope.json`. This refresh needs
the local census evidence.

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
network access. Each call runs in a named `rbr-msvc-*` container. A call that
exceeds `RBR_MSVC_TIMEOUT` seconds (default 300) fails, and its container is
removed. A hang usually means a compile error. Source and compiler mounts are read-only; only the output
directory is writable. Each call uses a fresh Wine prefix.
Prefix setup costs about 5 s per container. `check-overlay`, `progress`, and
`build` compile every MSVC source in one container, with up to eight parallel
`cl.exe` jobs that share one fresh prefix. Each job keeps its own timeout.
Clang sources compile in parallel. All objects are still rebuilt on every run.

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

Keep the shared Clang default until broader evidence supports a change. Coordinate
changes to config/msvc71.json, tools/docker/msvc/Dockerfile, and tools/qualify_msvc.py.
On another host, run `make qualify-msvc check-msvc-link` before claiming runner
support. Runtime behavior and the full source link remain unproved.
