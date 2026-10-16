struct Child004c4b20 {
    unsigned char unknown_00[0x214];
    unsigned int field_214;
};
struct Object004c4b20 {
    unsigned char unknown_00[4]; Child004c4b20 *field_04;
    unsigned int sub_004c4b20();
};
static_assert(__builtin_offsetof(Object004c4b20, field_04) == 4, "field offset");
static_assert(__builtin_offsetof(Child004c4b20, field_214) == 0x214, "field offset");
unsigned int Object004c4b20::sub_004c4b20() { return field_04->field_214; }
