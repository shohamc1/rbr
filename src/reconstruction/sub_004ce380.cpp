// sub_004ce380: EXE 0x004ce380, 19 bytes. thiscall member returning float.
//
// Unsigned-to-float view of the dword field at +0x50 (the same field the
// dword getters at 0x004ce368/0x004ce378 read): the value is loaded into
// EAX for the sign test, converted with FILD straight from the field, and
// 2^32 (shared constant-pool float at 0x00776a38, the pool that also holds
// 1.0f at +4) is added when the high bit is set, leaving the corrected
// unsigned value in ST(0). The volatile sign read keeps the two memory
// loads separate; a plain CSE'd read spills the integer to the stack
// before the FILD.
//
// NONMATCHING (2 bytes): the reference opens with PUSH ECX and closes with
// POP ECX — the 4-byte local slot MSVC's own unsigned-to-float idiom
// reserves — while every other byte and instruction is identical. Writing
// the idiom directly as `(float)*(unsigned int*)(this + 0x50)` reproduces
// the frame exactly but emits an object-local __real@4f800000 constant the
// checker cannot resolve, so the bias is referenced by its pinned pool
// address. The unused frame slot cannot be reserved from readable source:
// dead/address-taken/volatile locals are eliminated or add instructions
// (tested: local int, &local, volatile char decl/read/write, float locals,
// ternary forms, #pragma optimize("s"/"g"), /O1, /Op, /G5, /G7, /GA,
// /Og-, /Oi- — 11 recorded shapes).
class Object004ce380 {
public:
    float sub_004ce380();
};

float Object004ce380::sub_004ce380() {
    int sign = *(volatile int *)((char *)this + 0x50);
    float value = (float)*(int *)((char *)this + 0x50);
    if (sign < 0) {
        value += *(const float *)0x00776a38;
    }
    return value;
}
