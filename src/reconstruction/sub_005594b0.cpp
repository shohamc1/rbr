struct Object005594b0 {
    unsigned char unknown_00[8];
    unsigned int field_8;
    void sub_005594b0();
};
static_assert(__builtin_offsetof(Object005594b0, field_8) == 8, "field offset");
void Object005594b0::sub_005594b0() { field_8 = 0; }
