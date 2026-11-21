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
  the two `make qualify` candidate probes still differ, as before.
- Ghidra 12.2 replaced 12.1.4 on 2026-11-21. After two refinement passes, the EXE
  census has 27,201 entry candidates (27,207 before) and pseudocode covers 26,508
  functions in seven modules (26,642 before). MSVCP71 changed most: 125
  noncontiguous bodies (13 before) and 56 cross-function jumps (271 before). The
  matching project indexes 15,706 EXE functions.

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

All use the pinned MSVC 7.1 profile with `/O2 /Zl` unless noted.

| Function | Reference bytes | Compiled bytes | Remaining difference |
| --- | ---: | ---: | --- |
| `sub_00408d00` | 56 | 58 | Buffer-clear metadata stores and zero-register allocation |
| `sub_004183d0` | 44 | 41 | Reader register allocation, load order, and store order |
| `sub_0043ebb0` | 59 | 67 | List-tail reloads and separate head/tail paths |
| `sub_00402b50` | 96 | 96 | Compiler folds the member `operator<<(float)` import (IAT 0x70c274) into EDI with one load and three `call edi`; the reference keeps three `call [imm]` sites. Split statements, explicit call syntax, local dllimport pointers, `optimize("g"/"t")`, /G5, /G7, and /Oi all fail or lose the push-run. Hypothesis: 13.10.3077 folds at three call sites; the reference build may have required four. |
| `sub_004336b0` | 25 | — | Inputs arrive in AX, DX, CX plus one stack word (Borland-style register convention); no MSVC 7.1 declaration binds an input to EAX. Nothing in the EXE references it: dead code from a likely foreign-compiler object. The cdecl draft retains the packing semantics. |
| `sub_004c5d80` | 77 | 77 | Insert-path argument temps swap EAX/EDX homes: compiled loads `[esp+8]` into EAX and `[ecx+8]` into EDX, reference the reverse; 12 shapes tried including volatile. The EDX home likely came from the original inlining context. Everything else is byte-identical. |
| `sub_0067f514` | 31 | 31 | 2-byte residual: lea scratch register (`lea ecx` vs `lea edx`), stable across every shape, /O2, "s"/"gs" pragmas, and a thiscall-member definition. ECX is a register input (container); forwards to `sub_00679212` (`RET 0x10`, reads ECX). |
| `sub_0067f532` | 95 | 95 | Pre-call index materialization mirror only; byte-exact from `CALL sub_0067f514` onward. 18 recorded attempts. |
| `sub_0067ce0d` | 64 | 64 | Value/index register cycle plus prologue ordering; each flag-set configuration reproduces half the reference. Stop rule at 15 differing instructions. |

For `sub_00408d00`, loops, a memset intrinsic, and nested/volatile forms were tried.
The plain memset draft is retained after eight recorded attempts. The reader
loads three words before advancing its byte cursor and writing the output. The
list draft handles empty and nonempty insertion paths. Earlier forms and total
attempt counts for the reader and list were not preserved; do not claim an
eight-attempt history for them. Regenerate instruction dumps and check logs locally.

## Batch-100 subsystem findings (branch decomp/batch-100)

Wave 1 (clusters 0-4, 51 functions; 49 matching, 2 drafts above). Compact
subsystem identities, each supported by matched bodies:

- `sub_00402970`/`sub_00402980`: SSO string class with vtable at 0x0070c79c; the
  MSVCP71 `basic_string` body sits at +0x10 behind a 0x10 prefix.
  `sub_00402970` is the c_str getter (`cap`/`buf` split at +0x24/+0x10;
  `if (cap >= 0x10) return ptr; return buf;` matches where a ternary hoists
  the LEA). `sub_00402c20`/`sub_00402c80` are character/string log sinks
  (FILE* at +0x3c, flags at +0x40) inside a streambuf-like record at
  0x0076f428. `sub_004060c0` is a cdecl `floor(float)` wrapper;
  `sub_00406040`/`sub_00406080` need `/arch:SSE`.
- `sub_004336d0` is half of a 64-bit version pack/compare pair
  (`sub_004336b0` packs; callers compare against 0x00050001_08d20190).
  `sub_00433700`-`sub_004337f0` are the stdio backend of a cdecl file
  callback interface (open/read/write/tell/seek/close/error/user_data,
  slots 0-7; default table built by `sub_0041f4f0`, second filler at
  0x00433800 unclaimed). `sub_00433840` takes its object in EDX (dead ECX)
  at both call sites (0x00433928, 0x00435014).
- `sub_004491e0`-`sub_004492b0`: intrusive-list wrapper family. Vtable at +0,
  node base at +4 (links +4/+8, owner +0xc); finalizers store their vtable
  constant, decrement owner+8, and tail-jump one of four identical 93-byte
  unlink methods (`sub_004477b0`/`sub_00447850`/`sub_004478f0`/
  `sub_00447710`, manifest `source:null`). Vtables 0x70e964/0x70e970/
  0x70e97c/0x70e988 put the deleting finalizer in slot 0. `sub_0048cc90`
  (7 bytes) and `sub_0044cfe0` complete the logging-core neighborhood.
- `sub_0046e0d0`-`sub_00471f90`: one screen/coordinate class with a 0x17c-byte
  dispatch table at 0x00710734 (stored by SEH constructors 0x0046f960/
  0x0046ff80). Slots 0x14c/0x150 are int-to-float ratio converters
  (constants 360.0, 64.0, 480.0 at 0x710724/0x710728); slots 0x154/0x158 are
  the float-to-int inverses via `_ftol`. `sub_0046e270`/`sub_0046e290` are
  `__stdcall` table entries indexing the container at 0x00874618 through
  `sub_004ba120` (records at base+0x14c+index*0x1c). `sub_00471f90` is a
  cursor writer (memcpy intrinsic plus `field_04 += count`).
- `sub_00499180`-`sub_0049d010`: hierarchical intrusive-list/allocator
  subsystem. Containers keep a sentinel node at +4 and count at +8
  (`sub_00499270` inlines `sub_00499180`'s reset). Outer nodes flag at +0x1d,
  inner nodes (list embedded at +0x10) at +0x2d. Recursive node-tree release
  `sub_00498ea0` and inner variant `sub_004432b0`, iterator `sub_00498010`
  are manifest `source:null`. `sub_004995c0` seeds vtable 0x007137b4.

New encoding levers proven on this artifact (wave 1):

- Binding a subobject as a C++ reference (`Node &node = ...`) pins the exact
  store-vtable/load-owner/test schedule where pointer locals, volatile, /O1,
  /Os, and `optimize("s"/"g")` all let /O2 hoist the load above the store.
- A `MOV EAX,ECX` prologue identifies a constructor where the scaffold guessed
  a plain setter; constructor symbols then need `??0...` mangling.
- `while (--count >= 0)` is the load-bearing form for dec/js loop guards.
- MSVC 7.1 allocates xmm registers in reverse definition order (z,y,x temps).
- Declaration order of an index local versus a pointer local swaps xor/lea
  emission order in scan loops.

Wave 2 (clusters 5-9, 49 functions; 45 matching, 4 drafts above):

- `sub_004c1da0`-`sub_004c1f40`: one container family of 0x10- and 0x124-byte
  records plus 0x50-stride array accessors at +4. Destination-returning
  signatures are load-bearing for EAX homing in four copy helpers.
  0x004c4170 is the `add ecx,8; jmp [string dtor import]` destructor of a
  class with basic_string at +8. `sub_004c5d80` is a push_back for a
  4-byte-element container (+4 start, +8 end, +0xc storage-end).
- `sub_0050a8f0`/`sub_0050a910`: scalar deleting destructors (vtable slot 0 of
  0x0071e03c/0x0071e094). `sub_0050ab80` combines two tables via a kernel
  difference scaled by 20.3718319 (float at 0x0071e09c). The six 42-byte
  `sub_0050bd40`-`sub_0050be30` forwarders pass (a,b) through backend vtable
  slots 39/40/43/44/47/48 and return the first argument.
- `sub_005524f0`/`sub_005526d0`: quickrace event callbacks (registration at
  0x00552642/0x00552d5b) writing "qr%02d_in.arp"/"qr%02d_out.arp" names. The
  `sub_005538xx` family (fields +0x38/+0x4c/+0x468) has no EXE references;
  its dispatch mechanism is unexplained. `sub_00553930` copies the packed
  14-byte string "Richard Burns\0" from `gRichardBurnsName` (0x007270b8).
- `sub_005ebdb0`-`sub_005ef9e0`: container header (cursor +0, dword +4, flag
  byte +8, base +0xc) shared across strides 0x50/0xb4/0x78/0x28, plus a
  delegate record (raw-immediate table 0x007309c8 at +0, 3-pointer link at
  +4, receiver +0x10, function +0x14) with invoker `sub_005ed120` and
  thiscall list-unlink `sub_0043d920`.
- `sub_006769d5`-`sub_00688b6c`: size-optimized region — six of nine need
  `#pragma optimize("s", on)` (INC-pair adds, memory-bound compares,
  memory-push arguments, POP cleanup, re-pushed `[ESP+N]` arguments).
  `sub_0067bbb2`/`sub_0067bc0c` share receiver class `Object0067bbb2`
  (container: node count +0x08, record table +0x10, node table +0x14).

Additional wave-2 encoding levers:

- cdecl aliasing preserves ECX=this when the aliased call is the first
  action; `virtual` slot declarations reproduce `call [eax+N*4]` where a
  non-virtual declaration devirtualizes to a direct REL32 call.
- Sparse `switch {0,1,10}` gives the SUB 3/SUB 2 JE chain where `||` gives
  CMP/CMP; `return ++field` produces the one-byte INC EAX roundtrip.
- Struct copy from a named extern global folds to absolute per-field loads
  plus LEA dst; a cast constant pointer materializes a base register.
- Returning an otherwise-ignored value flips register allocation of the
  whole body; binding an embedded subobject to a local pointer hoists
  `ADD ECX,4` above the guard for ADD/DEC/JMP tail-call shapes.
- `/O2` naturally spills x87 intermediates into dead argument slots and
  interleaves `fmul [mem]` between epilogue pops.
- `#pragma pack(2)` prevents a trailing-word struct over-copy as dword.

Measurement caveats from this batch: Ghidra packets dropped `add esp,4`
after `call 0x006efdd0` and truncated `sub_00449290` by 3 bytes — always
verify packet listings against raw EXE bytes. MSVC 7.1 has no
`static_assert` or `__builtin_offsetof`; use typedef-array assertions.

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

## Logging, strings, and platform teardown

`sub_0048cc00` is the logging core `(kind, code, message, args)`. If
`gLogToStdout` (0x007a18f8) is set, it calls `vprintf`. Otherwise
`gLogRouteTable[code * 8 + kind]` (0x007a29d8; note the order) selects an 8-byte
record at 0x007a5ab4. The record's sink object receives `vsprintf` output from
the buffer at 0x007cc4b0 through virtual slot 1. Names are hypotheses.

`sub_0059e9d0` narrows a 16-bit string, mapping any character above 0xff to 0xff.
It matches only with the character cached in a `u16` local and tested as
`(c & 0xff00) > 0`; a direct test narrows to `test ch,0xff`. `sub_0059ee80` widens
with a do-while; its mid-span JMP is compiler loop alignment.

`GamePlatform0044a3c0::sub_00479e00` is teardown called from game main at
0x0047159d. It calls `sub_004d6210` (cdecl, no inputs; writes soundtweak.cfg and
deletes the object at 0x008cadb0), then releases the handles at +0x5c and +0x60
through virtual slot 27 of `gObject00874618`. `sub_00446bd0` assigns the string at
+0xd64 through the MSVCP71 `basic_string::operator=` import. MSVC 7.1 ends an
argument list whose last parameter is a class type with `@@@Z`; check other
manifest symbols with that shape.

## Shared objects and class files

Classes group functions only where callers pass the same object as `this`; see
the evidence comment in each class file. `0x007ca2a0` holds the
GamePlatform0044a3c0 instance (stored at 0x00471404). `0x007c9fe8` is the
application object: +0x28 is the Object0040e570 window object and +0x2c an
Object00406a20 instance. `0x007aeb78` is a state object whose +8 word
`sub_005341e0` sets and `sub_005342a0` and `sub_00534390` test.

`sub_0047a0a0` returns GamePlatform+0x70, which callers pass as `this` to
`sub_004837b0`, so an embedded object of at least 0x4f4 bytes starts there.
`sub_00587070` returns an array of 0x46c-byte elements whose type has
`sub_00586c20` and `sub_00586c70`. `sub_005b5550` indexes the container at
0x007b5a04 with stride 0xe8; the element class holds the five `sub_005c2f90`
to `sub_005c3030` accessors.

Likely merges that wait on test edits: Collection00416390 with Collection004163c0
(the same receivers and a 0x74-byte record), and Reader004183d0 with
Object004181e0. Nine sources are pinned by test includes or fixtures. Cross-profile
pairs (`sub_00406a20` with `sub_004087a0`, `sub_004c4a70` with `sub_004c4b00`,
`sub_004c41c0` with `sub_004c4240`) share receivers but stay split by compiler
profile. Shared headers use `layout.h` assertions, which compile under both
Clang and MSVC 7.1.

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
