// sub_004062a0: MSVC 7.1 (/O2 /Zl).
//
// atan of a float argument: FLD [ESP+4]; FLD1; FPATAN; RET. /O2 implies /Oi,
// and the atan intrinsic expands inline to FPATAN, which is why the EXE
// imports no atan. Part of the 0x004060xx math helpers; __cdecl.
extern "C" double __cdecl atan(double);
#pragma intrinsic(atan)

// FUNCTION: RBR 0x004062a0
extern "C" float __cdecl sub_004062a0(float value) {
    return (float)atan(value);
}
