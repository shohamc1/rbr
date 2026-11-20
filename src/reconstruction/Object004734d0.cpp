// Object004734d0: one object, reached through the global at 0x007ca200.
//
// Merge evidence: EXE code loads the same global as `this` for sub_004734d0
// (0x0056fbf2) and for sub_004736b0 (0x00531861, 0x005318ea, 0x0049439d,
// 0x005d4a7d). The field offsets do not overlap. Names are hypotheses.
// sub_004739a0, which the same global also feeds, is a free function.
struct Object004734d0 {
    unsigned char unknown_00[0x3c];
    unsigned int field_3c;
    unsigned char unknown_40[0xa8];
    unsigned int field_e8;

    void sub_004734d0();
    unsigned int sub_004736b0(unsigned int value);
};
static_assert(__builtin_offsetof(Object004734d0, field_3c) == 0x3c, "field offset");
static_assert(__builtin_offsetof(Object004734d0, field_e8) == 0xe8, "field offset");

void Object004734d0::sub_004734d0() { field_e8 = 0; }
unsigned int Object004734d0::sub_004736b0(unsigned int value) { return field_3c + value; }
