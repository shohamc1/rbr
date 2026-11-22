// sub_004062a0: EXE 0x004062a0, 9 bytes. MSVC 7.1 (/O2 /Zl).
//
// NONMATCHING draft. The reference is a bare x87 sequence with no call:
//   FLD dword ptr [ESP+4]; FLD1; FPATAN; RET
// i.e. atan(value) computed inline (FPATAN computes arctan(st(1)/st(0)) and
// leaves the double result in ST(0)) — the classic inline-assembly fast-math
// idiom of the 0x004060xx math neighborhood. MSVC 13.10.3077 has no
// intrinsic, expression, or CRT form that emits FPATAN inline; every C++
// route (atan/atanf/atan2) is a call, and the EXE imports none of them
// (MSVCR71 math imports are floor, ceil, frexp, ldexp and the _CI* helpers).
// The reference was either inline asm or a foreign compiler. This cdecl
// float-in/float-out draft compiles to FLD [ESP+4]; RET (5 bytes) so the
// four-byte FLD1/FPATAN residual stays visible; no unresolved symbols.

extern "C" float __cdecl sub_004062a0(float value) {
    return value;
}
