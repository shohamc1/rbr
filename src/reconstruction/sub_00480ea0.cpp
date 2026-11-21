// sub_00480ea0: EXE 0x00480ea0, 27 bytes. __thiscall member with one stack
// argument (RET 4), x87 result in ST(0).
//
// Three-component dot product: this . v with the argument's components
// loaded first (FLD [EAX+8], FMUL [ECX+8], FLD [EAX+4], FMUL [ECX+4], FADDP,
// FLD [EAX], FMUL [ECX], FADDP), so the z product sits deepest on the x87
// stack. x87 codegen: no /arch:SSE. Member of the vector family around
// 0x00480e80-0x00480f60; no incoming references are known, the layout is
// body evidence.

class Object00480ea0 {
public:
    float field_00;
    float field_04;
    float field_08;

    float sub_00480ea0(const Object00480ea0* v);
};

float Object00480ea0::sub_00480ea0(const Object00480ea0* v) {
    return field_00 * v->field_00 + field_04 * v->field_04 +
           field_08 * v->field_08;
}
