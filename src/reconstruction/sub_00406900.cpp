struct Object00406900 { unsigned char unknown_00[0x28]; unsigned int field_28; void sub_00406900(unsigned int); };
static_assert(__builtin_offsetof(Object00406900, field_28) == 0x28, "field offset");
void Object00406900::sub_00406900(unsigned int value) { field_28 = value; }
