struct Object004dde40 {
    unsigned char unknown_00[0x34]; unsigned int field_34;
    unsigned int sub_004dde40();
};
static_assert(__builtin_offsetof(Object004dde40, field_34) == 52, "field offset");
unsigned int Object004dde40::sub_004dde40() { return field_34; }
