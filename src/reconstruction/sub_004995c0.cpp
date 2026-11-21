// Object004995c0 constructor: EXE 0x004995c0, 51 bytes. __thiscall with no
// stack args, ECX-in/EAX-out per the observed prologue.
// Seeds field_00 with the vtable candidate at EXE 0x007137b4 (first slot
// 0x0049ab40), field_10 with 3, and the remaining words with zero, following
// the raw-vtable idiom of the matched sub_004495b0 (vtable 0x0070e994).
// Caller: sub_00450a10 at 0x00450a13.

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

Object004995c0::Object004995c0()
{
    vtable = 0x7137b4;
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
