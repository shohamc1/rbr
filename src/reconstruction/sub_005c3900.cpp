struct Object005c3900 {
    unsigned int field_00;
    void sub_005c3900(unsigned int value);
};
static_assert(__builtin_offsetof(Object005c3900, field_00) == 0, "field offset");
void Object005c3900::sub_005c3900(unsigned int value) { field_00 = value; }
