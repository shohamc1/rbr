// sub_0061e88a: EXE 0x0061e88a, 71 bytes. MSVC 7.1 (/O2 /Zl /Oy- /QIfist).
//
// Piecewise-curve member, the sub_0061e83a shape without the helper call:
// scales the argument by itself and the constant at 0x0072db70, truncates
// to an int index, and combines the two consecutive table entries at
// 0x00733c90 as
//   table[index] - table[index+1] + table[index+1] * frac
// where frac = scaled - (float)(unsigned)index; MSVC expands that
// conversion to FILD/JGE/FADD with the 2^32 pool float at 0x00776a38. __thiscall with
// one float stack word (RET 4); ECX is unused; result in ST(0). Callers:
// 0x0061f090, 0x0061f0b9, 0x0061f0c8, 0x0061f0d6.
//
// NONMATCHING (best draft, score 19): the reference truncates with FISTP
// DWORD [EBP-4] and reserves its one temp with PUSH ECX; MSVC 7.1 /QIfist
// emits FISTP QWORD (probe-verified on minimal casts across /Op, /O1,
// /Og-, /G5), which forces SUB ESP,8, keeps the product on the x87 stack
// (FLD ST(0)) instead of the reference's FSTP [EBP+8]/FLD round trip, and
// closes with MOV ESP,EBP rather than LEAVE. The tail evaluates
// (scaled - converted) before the table difference with FXCH/LEA/memory
// forms where the reference keeps Q on the x87 stack (FSUB ST(1), FILD,
// JGE, FADD, FSUBR, FMULP, FADDP). The 0x61c000-0x626000 curve subsystem
// has 110 FISTP-m32 sites, zero FISTP-m64 sites and zero __ftol calls,
// while the rest of .text makes 662 __ftol calls, so this file's original
// build is a different compiler vintage or an unknown flag.

class Object0061e88a {
public:
    float sub_0061e88a(float value);
};

extern "C" const float gCurveTable00733c90[]; // EXE .rdata 0x00733c90

float Object0061e88a::sub_0061e88a(float value) {
    float square = value * value;
    float scaled = square * 254.0f;
    int index = (int)scaled;
    const float* table = gCurveTable00733c90;
    return table[index] - table[index + 1]
         + table[index + 1] * (scaled - (float)(unsigned int)index);
}
