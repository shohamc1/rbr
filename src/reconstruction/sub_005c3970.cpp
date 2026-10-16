struct Object005c3970 {
    unsigned char unknown_00[4];
    unsigned int field_04;
    void sub_005c3970(unsigned int value);
};
static_assert(__builtin_offsetof(Object005c3970, field_04) == 4, "field offset");
void Object005c3970::sub_005c3970(unsigned int value) { field_04 = value; }
