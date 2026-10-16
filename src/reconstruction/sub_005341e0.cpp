struct Object005341e0 {
    unsigned char unknown_00[8];
    unsigned int field_8;
    void sub_005341e0(unsigned int value);
};
static_assert(__builtin_offsetof(Object005341e0, field_8) == 8, "field offset");
void Object005341e0::sub_005341e0(unsigned int value) { field_8 = value; }
