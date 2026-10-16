struct Object004de000 {
    unsigned char unknown_00[0x15f8]; float field_15f8;
    float sub_004de000();
};
static_assert(__builtin_offsetof(Object004de000, field_15f8) == 5624, "field offset");
float Object004de000::sub_004de000() { return field_15f8; }
