struct Object004e2e40 {
    unsigned char unknown_00[0x34]; unsigned int field_34;
    unsigned int sub_004e2e40();
};
static_assert(__builtin_offsetof(Object004e2e40, field_34) == 52, "field offset");
unsigned int Object004e2e40::sub_004e2e40() { return field_34; }
