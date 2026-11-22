// sub_004060c0: MSVC 7.1 (/O2 /Zl).
//
// Widens the float argument to double on the x87 stack and tail-calls the
// MSVCR71 floor import (IAT 0x0070c5a0), leaving the double result in ST(0):
//   FLD [esp+4]; SUB ESP,8; FSTP qword [esp]; CALL [__imp_floor]; ADD ESP,8
// __cdecl: float in, double out, caller cleans the eight-byte argument slot.

extern "C" __declspec(dllimport) double __cdecl floor(double x);

// sub_004060e0: MSVC 7.1 (/O2 /Zl).
//
// Identical byte shape to the matched sub_004060c0: widens the float argument
// to double on the x87 stack and tail-calls the MSVCR71 floor import
// (IAT 0x0070c5a0), leaving the double result in ST(0):
//   FLD [esp+4]; SUB ESP,8; FSTP qword [esp]; CALL [__imp_floor]; ADD ESP,8
// __cdecl: float in, double out, caller cleans the eight-byte argument slot.

extern "C" __declspec(dllimport) double __cdecl floor(double x);

// FUNCTION: RBR 0x004060c0
extern "C" double __cdecl sub_004060c0(float value) {
    return floor(value);
}

// FUNCTION: RBR 0x004060e0
extern "C" double __cdecl sub_004060e0(float value) {
    return floor(value);
}
