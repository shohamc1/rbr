// Object004dde40: one object, reached through the global at 0x008cecc0.
//
// Merge evidence: EXE code loads the same global as `this` for sub_004dde40
// (0x00534acf, 0x00534ae2, 0x00534aed, 0x00534b53) and for sub_004dde90
// (0x0052be8e). The field offsets do not overlap. Names are hypotheses.
struct Object004dde40 {
    unsigned char unknown_00[0x34];
    unsigned int field_34;
    unsigned char unknown_38[0xad4];
    float field_b0c;

    unsigned int sub_004dde40();
    float sub_004dde90();
};
static_assert(__builtin_offsetof(Object004dde40, field_34) == 0x34, "field offset");
static_assert(__builtin_offsetof(Object004dde40, field_b0c) == 0xb0c, "field offset");

unsigned int Object004dde40::sub_004dde40() { return field_34; }
float Object004dde40::sub_004dde90() { return field_b0c; }
