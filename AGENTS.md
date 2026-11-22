# Richard Burns Rally (Windows x86): matching decompilation

Goal: C++ that MSVC 7.1 compiles to the bytes of the selected executable,
then a native port built from that source. Modernization is deferred.

## The rules

- The target is `original/nocd/richardburnsrally.exe`, pinned by SHA-256 in
  `reccmp-project.yml`. Assume compatibility with 1.0.2, as directed by the
  user. Target changes need an explicit request.
- Never change a hash, size, compiler pin, or check just to pass.
- `make test check` must pass before integration or a commit.
- Every match comes from compiled C++. Original bytes are never source.
- Never commit disc images, executables, DLLs, assets, SDK headers, compiler
  binaries, generated assembly, or original bytes. Keep `disc/`, `original/`,
  and `build/` ignored.

## Layout

| Path | Contents |
| --- | --- |
| `analysis/notes.md` | Manual findings and open drafts |
| `config/functions.csv` | Reviewed function boundaries: address and size |
| `config/msvc71.json` | Compiler archive, file hashes, and base flags |
| `config/symbols.csv` | Linker symbols for code and data without source yet |
| `reccmp-project.yml` | reccmp target: reference hash and data sources |
| `src/reconstruction/` | Recovered C++, grouped into translation units |
| `tools/` | Build, check, extraction, and the Wine wrappers for reccmp |

## Setup

You need Python 3.11, Docker with x86 emulation (tested with OrbStack), and
Homebrew `llvm` and `lld`. Run `make setup`, then
`make extract ISO='/absolute/path/to/disc.iso'`. MSVC 7.1 (13.10.3077)
compiles through Docker and Wine. `lld-link` links natively, because Wine's
emulated `link.exe` fails on a second input (see `analysis/notes.md`).

## The loop

1. Pick a function. Use Ghidra through the `ghidra-rpc` CLI for pseudocode,
   assembly, and references. Treat names, types, and boundaries as hypotheses.
2. Add its source to the translation unit that holds its address neighbours,
   in address order, under an annotation:

   ```cpp
   // FUNCTION: RBR 0x004021e0
   void* Object004021e0::sub_004021e0(unsigned int flags) {
   ```

   Put `// NONMATCHING` on the line above the annotation for a draft.
3. Add its reviewed size to `config/functions.csv`. If it was a stub, delete
   its row from `config/symbols.csv`.
4. Run `make check`. It builds, runs reccmp, and audits every function byte
   for byte. Run `make diff ADDR=0x004021e0` for one function's diff.
5. On a match, remove `// NONMATCHING`. Record durable findings in
   `analysis/notes.md`.

`make check` fails when a function without `// NONMATCHING` differs, and
reports a draft that starts to match. `make report` writes
`build/report.html`.

### Callees and data without source

Declare the callee or global in source with its real signature, then add a
row to `config/symbols.csv` with its address, linker symbol, and type
(`stub`, `library`, or `global`). Give `size` for read-only data that the code
reads past its first word. The build defines each row as a stub, and reccmp
pairs the stub with the original address. Imports resolve from the reference's
import table.

### Build flags

The base flags are `/O2 /Zl`. A unit that needs more starts with a line such
as `// Build flags: /arch:SSE`. Only functions with the same flags share a
unit. Add a flag only when the reference shows it, such as the `/GS` cookie.

## Source rules

`make test` enforces the first rule.

- Never write a code or data address as an integer. Declare a named extern and
  add it to `config/symbols.csv`, or write the literal (`0.0f`, `"rb"`).
  Vtables use `gVtable<address>`. Unknown data uses `g<Role><address>`.
- A draft must compute what the reference computes. Never use a placeholder
  body to make a residual visible.
- Try the compiler intrinsic first: `/O2` implies `/Oi`. Use inline assembly
  only for an instruction MSVC 7.1 can't express, such as CPUID.
- Keep uncertain layouts local and assert offsets with `LAYOUT_OFFSET` and
  `LAYOUT_SIZE` from `layout.h`.
- Keep functions in ascending address order inside a unit. A unit is a
  hypothesis about the original object file; split it when evidence says so.

## When a function will not match

If eight compiles in a row don't reduce the difference, stop changing source.
Keep a readable draft under `// NONMATCHING`, and record the remaining
difference and what you tried in the table in `analysis/notes.md`.

## Naming

Unknown functions use `sub_<8-digit lowercase VA>`; unknown fields use
`field_<hex offset>`. Once evidence supports a purpose, use PascalCase for
functions and types, `gPascalCase` for globals, and lowerCamelCase for fields
and locals. A rename can change linkage, so rerun `make check` after one.
Don't invent class relationships to force a match.

## Work together

Own only the units you claim. Coordinate edits to `config/*.csv`, shared
headers, `analysis/notes.md`, and `tools/` with the integrator. Preserve
others' edits. Commit subjects use a capitalized imperative of about 50
characters. Keep commits scoped, and don't push without a request.
