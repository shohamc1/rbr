struct Object0040e900 { unsigned char unknown_00[0x6c0]; unsigned int field_6c0[8]; void sub_0040e900(const unsigned int *); };
#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
typedef char field_offset_check[offsetof(Object0040e900, field_6c0) == 0x6c0 ? 1 : -1];
extern "C" void *__cdecl memcpy(void *, const void *, unsigned int);
#pragma intrinsic(memcpy)
void Object0040e900::sub_0040e900(const unsigned int *values) { memcpy(field_6c0, values, 32); }
