struct Object0059bbe0 {
    unsigned char unknown_00[4];
    unsigned int field_04;
    void sub_0059bbe0(unsigned int value);
};
static_assert(__builtin_offsetof(Object0059bbe0, field_04) == 4, "field offset");
void Object0059bbe0::sub_0059bbe0(unsigned int value) { field_04 = value; }
