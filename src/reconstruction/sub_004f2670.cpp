struct Object004f2670 {
    unsigned char unknown_00[0xc]; unsigned int field_0c;
    unsigned int sub_004f2670();
};
static_assert(__builtin_offsetof(Object004f2670, field_0c) == 12, "field offset");
unsigned int Object004f2670::sub_004f2670() { return field_0c; }
