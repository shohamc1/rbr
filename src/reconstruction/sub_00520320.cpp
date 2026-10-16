struct Object00520320 {
    unsigned char unknown_00[8];
    unsigned int field_8;
    void sub_00520320();
};
static_assert(__builtin_offsetof(Object00520320, field_8) == 8, "field offset");
void Object00520320::sub_00520320() { field_8 = 1; }
