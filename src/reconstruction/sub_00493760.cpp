struct Object00493760 {
    unsigned char unknown_00[36];
    unsigned int field_24;
    unsigned int sub_00493760();
};
static_assert(__builtin_offsetof(Object00493760, field_24) == 36, "field offset");
unsigned int Object00493760::sub_00493760() { return field_24; }
