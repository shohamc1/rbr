struct Object004736b0 { unsigned char unknown_00[0x3c]; unsigned int field_3c; unsigned int sub_004736b0(unsigned int value); };
static_assert(__builtin_offsetof(Object004736b0, field_3c) == 0x3c, "field offset");
unsigned int Object004736b0::sub_004736b0(unsigned int value) { return field_3c + value; }
