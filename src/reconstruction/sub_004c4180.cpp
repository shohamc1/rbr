struct Object004c4180 {
    unsigned char unknown_00[8]; unsigned char field_08;
    unsigned char * sub_004c4180();
};
static_assert(__builtin_offsetof(Object004c4180, field_08) == 8, "field offset");
unsigned char * Object004c4180::sub_004c4180() { return &field_08; }
