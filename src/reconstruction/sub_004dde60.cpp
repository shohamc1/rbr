struct Object004dde60 {
    unsigned char unknown_00[0xa00]; unsigned char field_a00;
    unsigned char * sub_004dde60();
};
static_assert(__builtin_offsetof(Object004dde60, field_a00) == 2560, "field offset");
unsigned char * Object004dde60::sub_004dde60() { return &field_a00; }
