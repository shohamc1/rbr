struct Object0040eb90 { unsigned char unknown_00[0x6e4]; unsigned char *field_6e4; unsigned char sub_0040eb90(); };
#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
typedef char field_offset_check[offsetof(Object0040eb90, field_6e4) == 1764 ? 1 : -1];
unsigned char Object0040eb90::sub_0040eb90() { return field_6e4[0x10]; }
