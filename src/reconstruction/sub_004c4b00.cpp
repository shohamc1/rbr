struct Object004c4b00 {
    unsigned char unknown_00[4]; unsigned char *field_04;
    unsigned char * sub_004c4b00();
};
static_assert(__builtin_offsetof(Object004c4b00, field_04) == 4, "field offset");
unsigned char * Object004c4b00::sub_004c4b00() { return field_04 + 0x24; }
