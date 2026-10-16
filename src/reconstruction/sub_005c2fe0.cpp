struct Object005c2fe0 {
    unsigned char unknown_00[132];
    unsigned int field_84;
    void sub_005c2fe0();
};
static_assert(__builtin_offsetof(Object005c2fe0, field_84) == 132, "field offset");
void Object005c2fe0::sub_005c2fe0() { field_84 = 0; }
