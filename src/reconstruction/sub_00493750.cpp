struct Object00493750 {
    unsigned char unknown_00[32];
    unsigned int field_20;
    unsigned int sub_00493750();
};
static_assert(__builtin_offsetof(Object00493750, field_20) == 32, "field offset");
unsigned int Object00493750::sub_00493750() { return field_20; }
