// sub_0050ab80: EXE 0x0050ab80, 95 bytes. MSVC 7.1 (/O2 /Zl).
//
// thiscall member (two stack arguments, returns a float in st(0), ret 8).
// The receiver holds two array bases: field_04 (records of at least 0x108+4
// bytes with byte stride 0x40 per index) and field_08 (records of at least
// 0x88+4 bytes with byte stride 0x38 per index). The method clamps
// (field_04[index].0x108 + addend) to a byte, stores it at
// field_08[index].0x88 (raising a zero result to 1), and returns the scaled
// difference of sub_00504230(value, record) and sub_00504230(value - 1,
// record) through the float constant at 0x0071e09c (20.3718319, part of the
// constant pool that follows the 0x0071e094 vtable).

struct Object0050ab80 {
    unsigned char unknown_00[4];
    void* field_04;
    void* field_08;
    float sub_0050ab80(int index, unsigned int addend);
    float sub_00504230(unsigned int value, unsigned int* record); // 0x00504230
};

float Object0050ab80::sub_0050ab80(int index, unsigned int addend) {
    unsigned int* record =
        (unsigned int*)((char*)field_08 + index * 0x38 + 0x88);
    unsigned int value =
        (*(unsigned int*)((char*)field_04 + index * 0x40 + 0x108) + addend) & 0xff;
    *record = value;
    if (value == 0) {
        value = 1;
    }
    float first = sub_00504230(value, record);
    float second = sub_00504230(value - 1, record);
    return (first - second) * *(float*)0x0071e09c;
}
