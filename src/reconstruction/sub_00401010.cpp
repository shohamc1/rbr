struct Object00401010 { unsigned char unknown_00[4]; unsigned char field_04; unsigned char sub_00401010(); };
#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
typedef char field_offset_check[offsetof(Object00401010, field_04) == 4 ? 1 : -1];
unsigned char Object00401010::sub_00401010() { return field_04; }
