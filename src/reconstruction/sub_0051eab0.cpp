struct Object0051eab0 {
    unsigned char unknown_00[16];
    unsigned int field_10;
    void sub_0051eab0(unsigned int value);
};
static_assert(__builtin_offsetof(Object0051eab0, field_10) == 16, "field offset");
void Object0051eab0::sub_0051eab0(unsigned int value) { field_10 = value; }
