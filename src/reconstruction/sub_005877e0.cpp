struct Object005877e0 {
    unsigned char unknown_00[0x11bc];
    unsigned int field_11bc;
    unsigned int sub_005877e0();
};
static_assert(__builtin_offsetof(Object005877e0, field_11bc) == 0x11bc, "field offset");
unsigned int Object005877e0::sub_005877e0() { return field_11bc; }
