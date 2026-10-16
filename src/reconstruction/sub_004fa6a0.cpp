struct Object004fa6a0 {
    unsigned char unknown_00[4]; unsigned int field_04;
    unsigned int sub_004fa6a0();
};
static_assert(__builtin_offsetof(Object004fa6a0, field_04) == 4, "field offset");
unsigned int Object004fa6a0::sub_004fa6a0() { return field_04; }
