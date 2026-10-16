struct Object004ddea0 {
    unsigned char unknown_00[0xb1c]; float field_b1c;
    float sub_004ddea0();
};
static_assert(__builtin_offsetof(Object004ddea0, field_b1c) == 2844, "field offset");
float Object004ddea0::sub_004ddea0() { return field_b1c; }
