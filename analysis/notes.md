# Reconstruction notes

Scope: the NoCD EXE pinned in `config/reference.json`. DLL addresses require a
module name. Current counts and coverage are in `analysis/function-census.json`;
module dependencies are in `analysis/module-scope.json`.

## Source assumptions

- The manifest pins reviewed spans. Their decoded bodies are contiguous and end
  in RET; reviewed branch targets stay within each span. Straight-line leaves
  also have aligned entries and INT3 padding. Ghidra names, prototypes, and class
  identities remain hypotheses. Local offsets do not identify classes.
- Indexed helpers require valid caller-provided storage and indices. Allocation
  extents are unknown. Do not give an unknown tail array a fixed one-element bound.
- `sub_00401010` returns a byte in AL; upper EAX bits are unspecified.
  `sub_00587830` returns a full EAX 0/1 value, represented as `unsigned int`.
- Constructor forms use the observed ECX input and EAX return of `this`.
  `sub_004495b0` stores EXE address `0x0070e994`, a candidate virtual table.
  Original class names and a complete link model remain unknown.
- `sub_004c4b20` reads a child field at offset `0x214`. Child identity and alignment
  remain assumptions. `sub_00587080` computes a wrapped 32-bit address with stride
  `0x46c`; this representation needs review for a native port.
- Target pointer-layout assertions apply to x86. Native host behavior tests use
  host pointer sizes and do not establish the Windows ABI or floating-point state.
- EXE `0x00404da0` and `0x0041f080` use unresolved register inputs. Review their
  callers before assigning a standard C++ calling convention.

## Specific behavior

`sub_006de8a0` processes four floats per loop, then a scalar tail. It first applies
an upper bound, then a source-plus-bias lower bound. The lower bound can exceed the
upper bound. The x87 status test selects the lower bound for equality and unordered
comparisons: keep `!(value > lower)`, including its NaN behavior. Guard `count - 3`
with `count >= 4`. Overlapping source/output ranges use forward processing order.
Channel validity and all pointer extents remain caller preconditions.

`sub_0040e900` copies eight dwords with forward REP MOVSD. Its C++ representation
uses the MSVC memcpy intrinsic. Caller non-overlap is unproved; partial overlap
has machine behavior that C memcpy does not define.

## Unmatched drafts

All three use the pinned MSVC 7.1 profile with `/O2 /Zl`.

| Function | Reference bytes | Compiled bytes | Remaining difference |
| --- | ---: | ---: | --- |
| `sub_00408d00` | 56 | 58 | Buffer-clear metadata stores and zero-register allocation |
| `sub_004183d0` | 44 | 41 | Reader register allocation, load order, and store order |
| `sub_0043ebb0` | 59 | 67 | List-tail reloads and separate head/tail paths |

For `sub_00408d00`, loops, a memset intrinsic, and nested/volatile forms were tried.
The plain memset draft is retained after eight recorded attempts. The reader
loads three words before advancing its byte cursor and writing the output. The
list draft handles empty and nonempty insertion paths. Earlier forms and total
attempt counts for the reader and list were not preserved; do not claim an
eight-attempt history for them. Regenerate instruction dumps and check logs locally.

## Module scope and function boundaries

Use the separate census projects in `build/census/<sha256-prefix>/`. They do not
change the matching project or function manifests. Every count is a count of
machine-code entry candidates. It includes thunks, compiler helpers, and exception
handlers. It cannot recover the number of original source functions or inlined
functions. All unexplained code-section bytes remain in the coverage denominator.
Computed-call totals include imports and virtual calls. A missing decoded call
target does not by itself establish a missing game function.

The EXE loads the base HedgeHog3D and LocoMotion DLLs through `HH_Create` and
`LM_Create`. EXE load sites are `sub_004b9450` and `sub_004b9640`; provider exports
are `HedgeHog3D.dll:0x10006100` and `LocoMotion.dll:0x10029bc0`. Normal imports
also require MSVCR71 and MSVCP71. EXE `sub_0041adf0` loads DBGHELP for crash dumps;
DBGHELP imports MSVCRT. Launcher `sub_00417ae1` can load Unicows. Their supplied
binaries have separate census and pseudocode outputs. Windows can load a system
copy instead of a bundled DLL. Actual loaded paths remain untested.

The EXE has optional load code for NPClient at `0x004c69f8` and ParticleTweakerD
at `0x005cfe5e`. Their binaries are missing and entry reachability is unresolved.
No supported load edge reaches EAX, MFC71, MFC70, MSVCI70, MSVCP70, or MSVCR70.
Do not call these files unused. `Misc/lng.dll` in `misc.rbz` is 75-byte language
text, not a PE image; EXE `sub_00436dd0` parses it. The five base RBZ archives
have no other DLL- or EXE-named members. Local import-name/ordinal checks found
all 1,221 symbols across 13 edges in the supplied export tables; this does not
establish ABI compatibility or runtime use.

Reviewed corrections are in `config/census.json`. The imported operator delete
returns; a false no-return contract truncated many EXE bodies. Seven text limits
stop RTTI table scans before labels that resemble code addresses. A clean EXE
census removed two false entries from text. Seven reviewed false splits join their
owners. The `_onexit` callback at EXE `0x006f0726` has explicit startup evidence.
Code/data corrections restore direct call targets, switch cases, loop paths, and
epilogues. EXE `0x006e130c` and `0x005c2028` are RET followed by LEA padding;
Ghidra falsely read each four-byte sequence as address `0x00498dc3`.

The bounded branch scan follows both conditional paths and code after an early
RET. It rejects indirect jumps, unknown direct-call targets, overlapping bodies,
interior instruction targets and unexplained gaps. Each end must be 16-byte
aligned or followed by complete INT3 alignment. Candidates need an aligned start
after INT3 or a decoded RET. The scan collects adjacent chains in one batch.
Ghidra must agree with each
accepted body span. These limits leave unpadded entries, shared tails, exception
handlers, and indirect flow for review. A complete scan is not an exact count.
Keep LocoMotion `Catch@1003996e` and `Catch@1003997a` separate: the latter has a
data reference from exception metadata at `0x1006212c` and calls `_CxxThrowException`.
HedgeHog3D JMP `0x10005e8c` had a false CALL_RETURN override. Its target
`0x10005f90` restores the frame and SEH chain of `0x10005cb0`. Both incoming
branches belong to that owner. Restore the JMP and merge the false epilogue
entry. The raw entry candidate at
`HedgeHog3D.dll:0x10155a60` is inside PFMUL at `0x10155a5d`. LocoMotion has
the same instruction sequence at `0x1004a05d`. The ModR/M byte 0xcc resembles
INT3 padding. The Ghidra instruction-boundary check rejects both false entries.

`make census-decompile` exports pseudocode for every current candidate in DLLs
with supported load paths. It records failures per address. Export success does
not prove correct types, boundaries, runtime behavior, or recovered C/C++.
DLL source matching still needs module-qualified manifests and a module-aware
checker. The current source checker selects the EXE. The full source link and
Windows gameplay remain unproved.
