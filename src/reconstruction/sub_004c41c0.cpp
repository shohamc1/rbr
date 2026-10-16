struct Object004c41c0 {
    unsigned char unknown_00[48]; float field_30;
    float sub_004c41c0();
};
static_assert(__builtin_offsetof(Object004c41c0, field_30) == 48, "field offset");
float Object004c41c0::sub_004c41c0() { return field_30; }
