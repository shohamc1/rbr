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

- Clang 23.1.6 replaced 23.1.1 on 2026-11-21. Every Clang-profile match reproduces;
  the two `make qualify` candidate probes still differ, as before. Ghidra 12.2 is
  installed, but the pin stays 12.1.4 until census and matching analysis rerun.

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

## Startup subsystem (entry 0x006f0208)

The checker resolves REL32 calls to manifest symbols and DIR32 `__imp_` imports
(IAT slots and `FF 25` import thunks); relocation-free objects are unaffected.
Both relocation forms retain their COFF addends. Resolution requires an
undefined external symbol; object-local and common symbols remain unsupported.
`make build` compiles drafts without extracting their code. Exact checks still
reject extra code/data sections. Clang qualification uses the same resolver.
MSVC SEH and C++ EH frames still need linker-aware checking: scope tables land
in `.rdata`, funclets in `.text$x`, with `.xdata$x` funcinfo and
`__CxxFrameHandler` references (see `sub_0040f9f0`).

Matched entry callees: `sub_006f078b` returns zero; `sub_006f0770`
`__setdefaultprecision` calls the `_controlfp` thunk 0x006f0414 with
`_PC_53`/`_MCW_PC`; `sub_006efecf` is CRT `_atexit` with the NEG/SBB/NEG/DEC
tail. Both CRT wrappers need a file-local `#pragma optimize("s", on)`: under
plain `/O2` MSVC 13.10.3077 cleans cdecl arguments with `add esp,N`, while the
reference uses `POP`-based epilogues.

`sub_00471110` is the game main: `__stdcall` with four stack arguments. It
reads `[Settings]` from RichardBurnsRally.ini — Fullscreen, RunIntro,
RunDiagnose, RunStartup through the `sub_0040f9f0` initializer family — and
builds a `GamePlatform0044a3c0` object of 0x1bd0 bytes from the `sub_006efa32`
allocation helper with constructor `sub_0044a3c0(hInstance)`. CPUID at
0x00471360 has no MSVC 7.1 source form without inline assembly and remains the
hard blocker. MSVCP71 `basic_string` imports reproduce with
`__declspec(dllimport)` specializations; `char_traits` must be a struct and
`allocator` a class for exact mangling.

`sub_0040f9f0` is C++-EH-guarded (scope 0x70617b → funcinfo 0x79b5f0);
37 of 99 bytes match with a plain non-EH draft, and the ternary conditional is
load-bearing for the null path. `sub_006f06e2` `__RTC_Initialize` needs
`__try`/`__except(1)` and size optimization. Its scope table remains outside
checker support. The source uses separate linker sentinels; both resolve to
EXE 0x00788080. It does not call the SEH prologue and epilogue as ordinary C
functions. The filter at 0x006f070f and handler restore at 0x006f0713 sit
inside its 68-byte span.

Raw boundary review includes code after early returns. `sub_00471110` ends
after RET 0x10 at 0x0047160b (1,278 bytes); its return at 0x00471589 does not
end the function. `sub_004807c0` ends after RET at 0x00480910 (337 bytes),
and `sub_0059ee80` after RET at 0x0059eea4 (37 bytes). `sub_00411bc0` ends
after RET 0xc at 0x00411c73 (182 bytes), before padding. The manifest uses
these spans, rather than summed Ghidra body-range sizes.

## Launch and crash startup helpers

The reference build uses `/GS`: the cookie lives at 0x007c9410 and the check
call targets `@__security_check_cookie@4` (fastcall decoration) at 0x006efe25.
`config/globals.json` maps data symbols, so `/GS` functions compile with
`"extra_flags": ["/GS"]`; `sub_00470f70` (the CRASHDUMP.TXT unhandled-exception
filter, 410 bytes) matches this way. Any function showing the cookie load or
check call needs the flag; MSVC 7.1 is opt-in `/GS`.

Matched in this frontier: `sub_006efea9` (CRT `__onexit`: -1 sentinel on
`*(void**)0x16e3510`, `jmp [__imp__onexit]` tail, else `__dllonexit` thunk with
table pointers 0x16e3510/0x16e350c), `sub_00470f00` (launches RBR Startup.exe
and closes both handles), `sub_0040fa60` (ini-block release: the field must be
copied to a local so ESI holds the pointer across both calls), `sub_00470e60`
(intro-launch teardown gated on the flag byte 0x007ca1ec), `sub_0043b3c0`
(variadic error forward to the logging core `sub_0048cc00`; header-free
`va_start` is `(char*)&message + 4`; EXE callers pass kind, code, message),
and `sub_006efa32` (MSVC `operator new`:
malloc/`_callnewh`/`std::_Nomemory` retry loop). The intro launcher
`sub_00470ea0` stays nonmatching: its three PROCESS_INFORMATION stores and two
tail stores sit outside the /O2 push-run interleave cadence; volatile, memset
expansions, chained assignments, and the size pragma do not move them.

Source-level encoding levers proven on this artifact: a file-local
`#pragma optimize("s", on)` switches MSVC 7.1 to pop-based cdecl cleanup,
re-pushes buffered arguments from `[esp+N]`, and jmp-rotates loops; plain `/O2`
buffers a first cdecl argument in ECX. C imports need
`extern "C" __declspec(dllimport)`; mangled MSVCP names reproduce from real
C++ declarations (`namespace std { __declspec(dllimport) void __cdecl
_Nomemory(); }` gives `__imp_?_Nomemory@std@@YAXXZ`; `basic_string` needs
`char_traits` as a struct and `allocator` as a class).

`sub_0041c8a0` (RunDiagnose chain) stays nonmatching: the reference passes the
handler in EDI — a register argument no standard declaration produces (default
handler 0x0041c820, FILE* global 0x016e3044). `sub_0040fab0` is really 156
bytes (RET 4 at 0x40fb49) and is C++-EH-guarded. Its helper
`sub_00411bc0` also reads the container from EDI, in addition to three stack
arguments. The draft does not model this register input. The helper walks
node links; its first stack node pointer is not a virtual table.
Naming any destructor also emits a local `??_G` deleting-dtor
COMDAT that breaks the one-function rule. Ghidra packets under-report extents:
verify body ends against RET and padding (`sub_0040fa60` is 34, not 31). The
MSVC Docker image has no standard headers. A compile error can hang the Wine
container; the runner removes it after `RBR_MSVC_TIMEOUT` seconds (default 300).

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
