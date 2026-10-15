// Evidence: 0x00401000 reads a 32-bit field at ECX+0x0c. Class identity is unknown.
struct Object00401000 {
    unsigned char unknown_00[12];
    unsigned int field_0c;
    unsigned int sub_00401000();
};
static_assert(__builtin_offsetof(Object00401000, field_0c) == 12, "field offset");
unsigned int Object00401000::sub_00401000() { return field_0c; }
