struct Object0040e740 { unsigned char unknown_00[0x6e0]; unsigned char field_6e0; void sub_0040e740(unsigned char); };
#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
typedef char field_offset_check[offsetof(Object0040e740, field_6e0) == 1760 ? 1 : -1];
void Object0040e740::sub_0040e740(unsigned char value) { field_6e0 = value; }
