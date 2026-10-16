struct Object00533f00 {
    unsigned char unknown_00[12];
    unsigned int field_c;
    void sub_00533f00(unsigned int value);
};
static_assert(__builtin_offsetof(Object00533f00, field_c) == 12, "field offset");
void Object00533f00::sub_00533f00(unsigned int value) { field_c = value; }
