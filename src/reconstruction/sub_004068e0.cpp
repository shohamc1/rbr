// Evidence: 0x004068e0 writes the stack argument to ECX+0x3c and returns with RET 4.
struct Object004068e0 {
    unsigned char unknown_00[60];
    unsigned int field_3c;
    void sub_004068e0(unsigned int value);
};
static_assert(__builtin_offsetof(Object004068e0, field_3c) == 60, "field offset");
void Object004068e0::sub_004068e0(unsigned int value) { field_3c = value; }
