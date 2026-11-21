// sub_00499600: EXE 0x00499600, 17 bytes. __thiscall, one stack word (RET 4).
// Returns this->field_30 + 0x58 + index * 0x10: an element pointer inside the
// sub-table the field_30 base addresses. Callers: sub_00452030 (0x00452001,
// 0x004520a5), sub_005358f0, sub_00535a00 (x2), sub_00535c1f area,
// sub_0056bc80, sub_0056be53, sub_0056c0cc area, sub_0056d770.

struct Object00499600 {
    unsigned char field_00[0x30];
    unsigned char* field_30;
    unsigned char* sub_00499600(int index);
};

unsigned char* Object00499600::sub_00499600(int index)
{
    return field_30 + 0x58 + index * 0x10;
}
