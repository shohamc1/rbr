struct Object004fa6b0 {
    unsigned char unknown_00[8]; unsigned int field_08;
    unsigned int sub_004fa6b0();
};
static_assert(__builtin_offsetof(Object004fa6b0, field_08) == 8, "field offset");
unsigned int Object004fa6b0::sub_004fa6b0() { return field_08; }
