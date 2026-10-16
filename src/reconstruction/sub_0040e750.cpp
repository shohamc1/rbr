struct Object0040e750 { unsigned char unknown_00[0x6e1]; unsigned char field_6e1; void sub_0040e750(unsigned char); };
#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
typedef char field_offset_check[offsetof(Object0040e750, field_6e1) == 1761 ? 1 : -1];
void Object0040e750::sub_0040e750(unsigned char value) { field_6e1 = value; }
