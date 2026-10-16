struct Object00587070 {
    unsigned char unknown_00[4];
    unsigned int field_04;
    unsigned int *sub_00587070();
};
static_assert(__builtin_offsetof(Object00587070, field_04) == 4, "field offset");
unsigned int *Object00587070::sub_00587070() { return &field_04; }
