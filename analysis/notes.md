# Reconstruction notes

Scope: the NoCD EXE pinned in `reccmp-project.yml`. DLL addresses require a
module name.

## Build and comparison

- Wine's emulated i386 `link.exe` 7.10.3077 fails on any second input file:
  `create_view: Assertion 'view->protect & VPROT_SYSTEM' failed`, then LNK1000
  in Pass1. Debian Wine 8.0 fails on i386 and amd64 images alike; Wine 10.0
  (trixie, i386) can't map `link.exe` at all (c0000018). One input links.
  Native `lld-link` links instead. It ignores the MSVC 7.1 CodeView symbol and
  line subsections, so reccmp can't pair annotations by line;
  `build/reccmp.csv` names each annotation's linker symbol instead.
- reccmp ends an original function at any `jmp [IAT]` it reads as an import
  thunk, which cuts functions that tail-call an import. The reviewed sizes in
  `config/functions.csv` prevent that.
- reccmp copies a stub's extent to the original. A read-only stub stays one
  word, because strings follow vtables in `.rdata` and would read as vtable
  offsets. A writable stub spans to the next listed address, so
  `gIntroProcess+4` (`hThread`, 0x007ca1e0) names the same variable twice.
  Rows that share an address (`__SEH_prolog`, `gRtcInitializersBegin`/`End`)
  share one stub.
- reccmp reads `L"%s_%s.dmp"` (0x0071090c) as the narrow string "%" unless a
  `// STRING:` annotation marks it.
- All 72 former Clang-profile functions match under MSVC 7.1 with typedef-array
  layout assertions. `sub_0040ea30` returns its byte field zero-extended
  (MOVZX), so it returns `unsigned int`; Clang had hidden this.
- Translation units (2026-11-22): the 427 functions are grouped into 140
  address-adjacent units. Neighbours within 0x1000 bytes with the same build
  flags share a unit; a unit splits where two drafts define the same class
  differently. All 393 matches survived. The boundaries are hypotheses, not
  recovered object files. Merging the conflicting partial class layouts is
  open work.

## Source assumptions

- `config/functions.csv` pins reviewed spans. Their decoded bodies are contiguous and end
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
- Pointer-layout assertions apply to x86 only.
- EXE `0x00404da0` and `0x0041f080` use unresolved register inputs. Review their
  callers before assigning a standard C++ calling convention.

- A Ghidra 12.2 census found 27,201 EXE entry candidates, including thunks and
  compiler helpers. It is not a count of source functions.

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
| `sub_00471790` | 81 | — | SIMD math library: second SHUFPS must rotate XMM0 in place with the accumulator in XMM1; 7.1 /O2 always copies to XMM2, re-homes the accumulator, and spills (EBP+AND frame). 7 shapes, ±/arch:SSE. |
| `sub_00463ff0`/`00464060`/`00480e80`/`00480ec0`/`00480ef0`/`004eb160`/`004eb180` | — | — | One SSE-math family (about 30 EXE sites): reference keeps `this` in a 4-byte stack slot (`PUSH ECX; MOV [ESP],ECX`) and rsqrt/shuffle copy shapes 7.1 never emits at /O2 (20+ shapes each; intrinsics without `align(16)` removed the frame but not the homing). Hypothesis: that library's objects were built with a different mode such as /GL. |
| `sub_0061e83a`/`0061e88a` | — | — | FISTP-m32 wall: the 0x61c000-0x626000 region has 110 FISTP-m32 sites, zero FISTP-m64, zero `__ftol` calls, while the rest of .text makes 662 `__ftol` calls. `/QIfist` always emits FISTP QWORD (probed with /Op /O1 /Og- /G5), so these two curve functions cannot match under the pinned toolchain. Neighbors in the same region are 7.1-exact: the vintage split is per source file. |
| `sub_0040e960` | 38 | 38 | Reference hoists `mov edx,[0x016e304c]` one instruction earlier than every two-statement shape; the right order recolors registers (16+ shapes). Loading the holder between the two flag stores fixes the order but recolors to EAX/DL in every form tried (local flag, early declarations; score 6). |
| `sub_00421a10`/`00421de0` | 87/93 | — | Identical one-byte residual: rotated advance loop's load base register (`mov eax,[ecx+K]` vs `[eax+K]`); stable across 13/11 shapes. |
| `sub_00492fe0` | 86 | 85 | Buffered byte-writer (one of three identical template bodies): reference materializes the address then loads the value byte into the dead index register; every /O2 shape hoists the value to AL and folds the store (~40 shapes). |
| `sub_004a5120` | 74 | — | x87 operand-form trilemma: memory-form `fadd [ecx+0x2c]` needs a volatile-leaf counterpart that contradicts the statement-split multiply; non-volatile restores forms but store-forwards. Reading the target `field_24[1]` into a local first reproduces the leading `fld [ecx+0x28]` but not the memory-form FADD (score 7). x87 operand rules are under the batch-200 levers below. |
| `sub_004ea1f0` | 74 | 74 | PUSH EDI/1.0f hoisted one instruction (8 shapes). |
| `sub_00503550` | 61 | 61 | Shared vtable const `mov edx,0x71135c` (0x0071135c holds code addresses; the four {vptr, 0} pairs look like embedded polymorphic members) materializes at first use; reference materializes at join entry (8 shapes + 3 pragmas). |
| `sub_00476270` | 109 | 109 | ECX homing slot schedule plus a custom ECX-receiving cdecl convention with no plain C++ form; its caller `sub_004767a0` matches via the cdecl alias (the reference call relies on the previous call preserving ECX — MSVC always reloads ECX between member calls). Manifest keeps the cdecl symbol `_sub_00476270`. |
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
  `sub_00447710`, stubs in `config/symbols.csv`). Vtables 0x70e964/0x70e970/
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
  are stubs in `config/symbols.csv`. `sub_004995c0` seeds vtable 0x007137b4.

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

## Batch-200 subsystem findings (branch decomp/batch-200)

178 of 200 functions matching across 21 clusters (including all 17 small
`source:null` callees that blocked batch-100 callers). Draft residuals are in
the table above.

- The 93-byte thiscall intrusive-list unlink now has eight byte-identical EXE
  copies (`sub_0043d920`, `sub_004477b0/50/f0/10`, `sub_004544b0`,
  `sub_00454ab0`, `sub_00454870`, quartet `sub_00454c30/c90/e70/e10`), one per
  wrapper node class; 16-byte thunks DEC the owner count and tail-jmp into
  them. Finalizer wrappers share the wave-1 shape (vtable +0, node +4,
  owner +0xc).
- The streambuf guard family (0x0041b250-0x0041b390) wraps MSVCP71
  `basic_streambuf::_Lock/_Unlock`: object+0 is the stream subobject,
  vbtable[1] gives the virtual basic_ios base offset, basic_ios+0x28 the
  guarded streambuf. The basic_ios import needs a 2-parameter template with a
  virtual dtor.
- 0x0043a120-0x0043a2c0 is a 4-byte-element container/allocator family
  (size/capacity/allocate/fill/move); only the fill has EXE call sites.
  `sub_004b04b0/50/20` are bare-node tail-appends whose single-tail source
  makes /O2 tail-duplicate the suffix exactly.
- The 0x0040e5xx family extends Object0040e570: stats record at +0x6e4
  (layout mapped to +0x8c) and a +0xf4 dispatcher whose entries are
  `__stdcall` virtual members (slots 0x6c-0x1ac). 0x016e304c is a pointer
  global beside the FILE* at 0x016e3044.
- Slot-machine state on object 0x007ca200 (`sub_00474950/76270/767a0`):
  4 slots at +0x100, state +0xe8, floats +0xec-0xfc; container family at
  0x004728cxx walks 0xc records skipping types 0x15-0x17.
- 0x00471790 region and 0x00480e80-0x004eb180: one SIMD math library with
  the /GL-class residuals above; `sub_004eb180` also proves 7.1 fixed points
  (`_mm_rsqrt_ss` always round-trips memory; same-operand shuffle always
  copies; intrinsics need extern "C").
- Quickrace/callback region (0x00545xxx-0x00569xxx): vtable pair
  0x7264e4/0x7265ac, deleting dtor slot 0 class (size 0xd8) with guarded
  cdecl callbacks at +0xd4; delegate-record finalizers clear receiver+0x10 /
  callback+0x14; `sub_0051c000` culls by SSE length; `sub_00569f70` is the
  cdecl quickrace callback on containers 0x0163f2a0/0x0163f0a8.
- 0x0048f4f0-0x0048f670: multi-group dispatch table 0x00712f70 (deleting
  finalizers at slots 0/3/6/9); 0x004809xx is a file-wrapper class (heap
  blocks +0x14/+0x18, HANDLE +0x20, filename +0x24).
- 0x004b3fd0 creates through global device `*(void **)0x008b4998` vtable
  slot 0x50 then seven calls at slots 0x50-0x8c (manager at 0x008b4998 also
  seen by `sub_004a5980`, vtable 0x0071463c).
- CRT identities: 0x006efe34 is `__ftol2` (7.1 emits `call __ftol2` for float
  truncation); IAT 0x0070c2d8 is the MSVCP71 basic_string dtor with
  `sub_0043d6e0` its +0xc adjuster thunk; `sub_006efdd0` is the operator
  delete thunk (`JMP [0x0070c504]`).

New encoding levers proven on this artifact (batch 200):

- One trailing clear in an unlink (zero-case arm first) makes /O2 duplicate
  the clear into all exits and cross-jump the shared store; early-return and
  size pragmas fail in opposite ways.
- A getter whose reference leaves the out pointer or `this` in EAX at RET
  originally returned that value: declaring the return type fixes register
  allocation exactly; no reshaping moves /O2 off EAX-first.
- `void* volatile out` parameters block CSE and flip a whole cdecl push-run
  schedule; PUSH ECX + MOV EAX,[ESP] + PUSH EAX is the idiom for passing an
  uninitialized 4-byte local (ECX there is dead storage, not an input).
- `__declspec(dllimport)` must precede the return type or MSVC calls a CRT
  thunk instead of the IAT slot; a second unused stack parameter blocks the
  MOV [ESP+4]/JMP tail-jump in operator-new wrappers.
- Entry `MOV EAX,ECX` in a setter means `return this` (`QAEPAV1@...`);
  `MOV ESI,ECX` prologues identify constructors.
- Explicit ctor call through a cast (`((Base*)this)->Base::Base()`) emits a
  bare CALL reusing the incoming ECX home; cdecl aliases preserve ECX across
  calls where member declarations always reload it.
- Sparse switches give SUB/DEC JE chains where `||` gives CMP/CMP; unsigned
  counters give JB not JL; `!(a >= b)` gives TEST AH,1/JNE; `do{}while` after
  `flag = 1` defeats loop inversion; dword-cursor typing fixes INC/ADD tails.
- Struct-typed parameters (by-value 12-byte triples) are load-bearing for
  LEA/EDX ping-pong; `p->~Class()` on a class-typed parameter avoids the ??_G
  COMDAT where void* emits one.
- `__m128` without `align(16)` keeps intrinsics inlinable without the EBP
  realignment frame; /arch:SSE changes nothing in intrinsic-only bodies.
- x87 operand forms: leaf*leaf multiply loads the right factor and
  memory-forms the left; a subtraction keeps the memory form on the right
  when the left is a volatile leaf or an arithmetic subexpression, or the
  right is plain; statement-split `d = d * field` memory-forms the right,
  an in-expression multiply never does; a volatile-cast read blocks
  load/store forwarding but always takes its own FLD.
- /Oy- per file restores EBP frames; `(unsigned char)` casts give MOVZX +
  signed FILD with no 2^32 fixup.
- `(float)unsignedValue` is the source of FILD/JGE/FADD with the 2^32 pool
  float at 0x00776a38 and the PUSH ECX slot (`sub_004ce380`).
- /O2 implies /Oi, and the atan intrinsic expands inline to FLD1/FPATAN
  (`sub_004062a0`); the EXE imports no atan.
- A float literal memory-forms each use (`fmul [__real@...]`). A reference
  that loads the constant once and reuses ST(n) read a named constant, so
  `sub_0061decb` keeps `gColorScale0070e9ec`. The reverse also holds: reading
  1.0f through a pointer hoisted the reciprocal in `sub_00406100`; the literal
  matches.

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

Float pools observed: 0.0f 0x00776a70, 1.0f 0x00776a3c, -1.0f 0x00776a48,
2^32 0x00776a38, 254.0f 0x0072db70. MSVC SEH and C++ EH frames are not checked
yet: scope tables land in `.rdata`, funclets in `.text$x`, with `.xdata$x`
funcinfo and `__CxxFrameHandler` references (see `sub_0040f9f0`).

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
after RET 0xc at 0x00411c73 (182 bytes), before padding. `config/functions.csv`
uses these spans, rather than summed Ghidra body-range sizes.

## Launch and crash startup helpers

The reference build uses `/GS`: the cookie lives at 0x007c9410 and the check
call targets `@__security_check_cookie@4` (fastcall decoration) at 0x006efe25.
`/GS` functions carry `// Build flags: /GS`; `sub_00470f70` (the CRASHDUMP.TXT unhandled-exception
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
symbols with that shape.

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

Likely class merges: Collection00416390 with Collection004163c0 (the same
receivers and a 0x74-byte record), Reader004183d0 with Object004181e0, and the
receiver pairs `sub_00406a20`/`sub_004087a0`, `sub_004c4a70`/`sub_004c4b00`,
and `sub_004c41c0`/`sub_004c4240`. Shared headers use `layout.h` assertions.

## Module scope

The EXE loads the base HedgeHog3D and LocoMotion DLLs through `HH_Create` and
`LM_Create`. EXE load sites are `sub_004b9450` and `sub_004b9640`; provider exports
are `HedgeHog3D.dll:0x10006100` and `LocoMotion.dll:0x10029bc0`. Normal imports
also require MSVCR71 and MSVCP71. EXE `sub_0041adf0` loads DBGHELP for crash dumps;
DBGHELP imports MSVCRT. Launcher `sub_00417ae1` can load Unicows. Windows can
load a system copy instead of a bundled DLL. Actual loaded paths remain untested.

The EXE has optional load code for NPClient at `0x004c69f8` and ParticleTweakerD
at `0x005cfe5e`. Their binaries are missing and entry reachability is unresolved.
No supported load edge reaches EAX, MFC71, MFC70, MSVCI70, MSVCP70, or MSVCR70.
Do not call these files unused. `Misc/lng.dll` in `misc.rbz` is 75-byte language
text, not a PE image; EXE `sub_00436dd0` parses it. The five base RBZ archives
have no other DLL- or EXE-named members. Local import-name/ordinal checks found
all 1,221 symbols across 13 edges in the supplied export tables; this does not
establish ABI compatibility or runtime use.
