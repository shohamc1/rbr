// sub_0051c000: EXE 0x0051c000, 93 bytes. MSVC 7.1 (/O2 /Zl).
//
// Range culling test on a scene-graph node: returns 1 when the vector at
// +0x190 is shorter than the inner radius read from *(this+0x34)+0x15c, or
// when the vector at +0x1c0 is shorter than the outer radius at
// *(this+0x34)+0x17c; both radii are kept in x87-compared stack slots and
// measured against the ST(0) length returned by the SSE helper sub_00471790
// (TEST AH,5 / JNP strict-less form; NaN counts as not-less). Otherwise
// restores this into ECX and tail-jumps the constant-false method
// sub_0050c240 (XOR EAX,EAX; RET), returning 0. thiscall member returning
// int, one local (PUSH ECX), bare RET. Vtable references 0x007203b8 and
// 0x00722f18; also reached by JMP from 0x0050fbd0 and 0x005131d0.

class Object00471790 {
public:
    float sub_00471790();   // external SSE vector length, EXE 0x00471790
};

class Object0050c240 {
public:
    int sub_0050c240();   // external constant-false method, EXE 0x0050c240
};

class Object0051c000 {
public:
    unsigned char unknown_00[0x34];
    void *field_34;   // settings record: float radii at +0x15c/+0x17c

    int sub_0051c000();
};

int Object0051c000::sub_0051c000() {
    float inner = *(float *)((char *)field_34 + 0x15c);
    if (((Object00471790 *)((char *)this + 0x190))->sub_00471790() < inner) {
        return 1;
    }
    float outer = *(float *)((char *)field_34 + 0x17c);
    if (((Object00471790 *)((char *)this + 0x1c0))->sub_00471790() < outer) {
        return 1;
    }
    return ((Object0050c240 *)this)->sub_0050c240();
}
