struct Object004eee30 {
    unsigned char unknown_00[0x67c]; unsigned int field_67c;
    unsigned int sub_004eee30();
};
static_assert(__builtin_offsetof(Object004eee30, field_67c) == 1660, "field offset");
unsigned int Object004eee30::sub_004eee30() { return field_67c; }
