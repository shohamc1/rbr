struct Object004087a0 { unsigned char unknown_00[0x28]; unsigned char field_28; unsigned char unknown_29[0x1b]; unsigned int field_44; void sub_004087a0(unsigned int); };
static_assert(__builtin_offsetof(Object004087a0, field_28) == 0x28, "field offset");
static_assert(__builtin_offsetof(Object004087a0, field_44) == 0x44, "field offset");
void Object004087a0::sub_004087a0(unsigned int value) { field_44 = value; field_28 = 1; }
