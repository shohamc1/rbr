struct Object00523150 {
    unsigned char unknown_00[20];
    unsigned int field_14;
    unsigned int sub_00523150();
};
static_assert(__builtin_offsetof(Object00523150, field_14) == 20, "field offset");
unsigned int Object00523150::sub_00523150() { return field_14; }
