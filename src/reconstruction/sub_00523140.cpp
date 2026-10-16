struct Object00523140 {
    unsigned char unknown_00[16];
    unsigned int field_10;
    unsigned int sub_00523140();
};
static_assert(__builtin_offsetof(Object00523140, field_10) == 16, "field offset");
unsigned int Object00523140::sub_00523140() { return field_10; }
