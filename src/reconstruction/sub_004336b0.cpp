// Four-lane u16 packer producing the 64-bit values compared by sub_004336d0
// (the version-compare family: its caller sub_00432c40 builds values such as
// 0x00050001_08d20190 from four 16-bit components). The reference body packs
// its inputs as EDX:EAX = (DX<<16|AX) : ([esp+4]<<16|CX), i.e. a 64-bit value
// (param2 << 48) | (param1 << 32) | (param4 << 16) | param3 read from the
// EAX, EDX, ECX registers and one stack word.
//
// NONMATCHING, blocked on the calling convention: that input layout is the
// Borland register convention (first three arguments in EAX, EDX, ECX, the
// rest on the stack), which no MSVC 7.1 declaration reproduces — a __fastcall
// 64-bit parameter goes on the stack (verified: @sub_004336b0@12), and no
// MSVC calling convention binds an argument to EAX. Nothing in the EXE
// references this function (no call, jump, or data reference anywhere in the
// image), so the convention cannot be recovered from a caller either; the
// body is likely dead code from a foreign-compiler object. The draft below
// keeps the packing semantics as a plain cdecl function; it compiles to stack
// loads instead of the register reads. Compiled with MSVC 7.1 (/O2 /Zl).
extern "C" unsigned __int64 __cdecl sub_004336b0(unsigned short param1,
                                                 unsigned short param2,
                                                 unsigned short param3,
                                                 unsigned short param4) {
    return ((unsigned __int64)param2 << 48) | ((unsigned __int64)param1 << 32) |
           ((unsigned __int64)param4 << 16) | param3;
}
