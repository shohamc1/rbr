struct Object00493770 {
    unsigned char unknown_00[40];
    unsigned int field_28;
    unsigned int sub_00493770();
};
static_assert(__builtin_offsetof(Object00493770, field_28) == 40, "field offset");
unsigned int Object00493770::sub_00493770() { return field_28; }
