struct Object005c2f90 {
    unsigned char unknown_00[140];
    unsigned int field_8c;
    unsigned int sub_005c2f90();
};
static_assert(__builtin_offsetof(Object005c2f90, field_8c) == 140, "field offset");
unsigned int Object005c2f90::sub_005c2f90() { return field_8c; }
