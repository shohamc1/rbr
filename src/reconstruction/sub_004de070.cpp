struct Object004de070 {
    unsigned char unknown_00[0x30]; unsigned int field_30;
    unsigned int sub_004de070();
};
static_assert(__builtin_offsetof(Object004de070, field_30) == 48, "field offset");
unsigned int Object004de070::sub_004de070() { return field_30; }
