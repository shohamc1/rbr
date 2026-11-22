// sub_00464010: EXE 0x00464010, 76 bytes. __thiscall three-float subtract
// returning the four-float record by value: ECX is the left operand, the
// hidden return-slot pointer is the first stack argument ([EBP+8]), the
// right operand pointer is the second ([EBP+0xc]). RET 8.
//
// The record class carries 16-byte alignment (__declspec(align(16))): the
// local result record forces the realigned EBP frame (AND ESP,~0xf). Only
// the first three components are computed; the fourth ([ESP+0xc]) is never
// written, and the whole 16-byte record including that undefined word is
// copied to the return slot. Callers use the returned EAX (the return slot)
// directly, e.g. 0x004efb2e (MOV ECX,EAX; CALL 0x0049ce70).

class __declspec(align(16)) Object00464010 {
public:
    float field_00;
    float field_04;
    float field_08;
    float field_0c;

    Object00464010 sub_00464010(const Object00464010& other);
};

Object00464010 Object00464010::sub_00464010(const Object00464010& other) {
    Object00464010 result;
    result.field_00 = field_00 - other.field_00;
    result.field_04 = field_04 - other.field_04;
    result.field_08 = field_08 - other.field_08;
    return result;
}
