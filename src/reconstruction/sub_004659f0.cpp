#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
struct Child004659f0 { unsigned char unknown_00[0x370]; unsigned int field_370; };
struct Object004659f0 { unsigned char unknown_00[0x70]; Child004659f0 *field_70; void sub_004659f0(unsigned int value); };
typedef char offset_check[offsetof(Object004659f0, field_70) == 0x70 ? 1 : -1];
typedef char offset_check[offsetof(Child004659f0, field_370) == 0x370 ? 1 : -1];
void Object004659f0::sub_004659f0(unsigned int value) { field_70->field_370 = value; }
