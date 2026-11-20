// Object004fa6a0: the member object at offset 0x58 of the object that
// function 0x00527420 receives in ECX.
//
// Merge evidence: that function copies ECX to ESI, then passes `[esi+0x58]` as
// `this` to sub_004fa6b0 (0x00527434, 0x00527463, 0x00527496) and to
// sub_004fa6a0 (0x00527476, 0x005274a9). Names are hypotheses.
struct Object004fa6a0 {
    unsigned char unknown_00[4];
    unsigned int field_04;
    unsigned int field_08;

    unsigned int sub_004fa6a0();
    unsigned int sub_004fa6b0();
};
static_assert(__builtin_offsetof(Object004fa6a0, field_04) == 4, "field offset");
static_assert(__builtin_offsetof(Object004fa6a0, field_08) == 8, "field offset");

unsigned int Object004fa6a0::sub_004fa6a0() { return field_04; }
unsigned int Object004fa6a0::sub_004fa6b0() { return field_08; }
