// Object004995c0 constructor: EXE 0x004995c0, 51 bytes. __thiscall with no
// stack args, ECX-in/EAX-out per the observed prologue.
// Seeds field_00 with the vtable candidate at EXE 0x007137b4 (first slot
// 0x0049ab40), field_10 with 3, and the remaining words with zero, following
// the raw-vtable idiom of the matched sub_004495b0 (vtable 0x0070e994).
// Caller: sub_00450a10 at 0x00450a13.

extern "C" const void* const gVtable007137b4[]; // EXE .rdata 0x007137b4

struct Object004995c0 {
    unsigned int vtable;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
    unsigned int field_10;
    unsigned int field_14;
    unsigned int field_18;
    unsigned int field_1c;
    unsigned int field_20;
    unsigned int field_24;
    unsigned int field_28;
    unsigned int field_2c;
    unsigned int field_30;
    Object004995c0();
};

// sub_00499600: __thiscall, one stack word (RET 4).
// Returns this->field_30 + 0x58 + index * 0x10: an element pointer inside the
// sub-table the field_30 base addresses. Callers: sub_00452030 (0x00452001,
// 0x004520a5), sub_005358f0, sub_00535a00 (x2), sub_00535c1f area,
// sub_0056bc80, sub_0056be53, sub_0056c0cc area, sub_0056d770.

struct Object00499600 {
    unsigned char field_00[0x30];
    unsigned char* field_30;
    unsigned char* sub_00499600(int index);
};

// sub_00499730: __stdcall leaf.
// Bytes-to-KiB ceiling divide: (n + 0x3ff) >> 10. Referenced from the data
// tables at EXE 0x0070f3b4 and 0x0071385c (vtable slots), so calls are
// indirect and Ghidra lists no direct callers.

// sub_00499740: __stdcall leaf.
// KiB to bytes: n << 10. Referenced from the data tables at EXE 0x0070f3b8
// and 0x00713860 (vtable slots), so calls are indirect.

// sub_00499780: __thiscall setter, one stack word.
// this->field_10 = value. Callers: sub_00451410 at 0x0045145f and
// sub_004514e0 at 0x00451505.

struct Object00499780 {
    unsigned char field_00[0x10];
    unsigned int field_10;
    void sub_00499780(unsigned int value);
};

// FUNCTION: RBR 0x004995c0
Object004995c0::Object004995c0()
{
    vtable = (unsigned int)gVtable007137b4;
    field_04 = 0;
    field_08 = 0;
    field_0c = 0;
    field_10 = 3;
    field_14 = 0;
    field_18 = 0;
    field_1c = 0;
    field_20 = 0;
    field_24 = 0;
    field_28 = 0;
    field_2c = 0;
    field_30 = 0;
}

// FUNCTION: RBR 0x00499600
unsigned char* Object00499600::sub_00499600(int index)
{
    return field_30 + 0x58 + index * 0x10;
}

// FUNCTION: RBR 0x00499730
extern "C" unsigned int __stdcall sub_00499730(unsigned int bytes)
{
    return (bytes + 0x3ff) >> 10;
}

// FUNCTION: RBR 0x00499740
extern "C" unsigned int __stdcall sub_00499740(unsigned int kbytes)
{
    return kbytes << 10;
}

// FUNCTION: RBR 0x00499780
void Object00499780::sub_00499780(unsigned int value)
{
    field_10 = value;
}
