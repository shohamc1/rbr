struct Object004dde90 {
    unsigned char unknown_00[0xb0c]; float field_b0c;
    float sub_004dde90();
};
static_assert(__builtin_offsetof(Object004dde90, field_b0c) == 2828, "field offset");
float Object004dde90::sub_004dde90() { return field_b0c; }
