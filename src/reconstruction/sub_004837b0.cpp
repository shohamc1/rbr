struct Object004837b0 { unsigned char unknown_00[0x4f0]; unsigned int field_4f0; void sub_004837b0(unsigned int value); };
static_assert(__builtin_offsetof(Object004837b0, field_4f0) == 0x4f0, "field offset");
void Object004837b0::sub_004837b0(unsigned int value) { field_4f0 = value; }
