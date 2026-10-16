#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
struct Object0047a460 { unsigned char unknown_00[0x724]; unsigned int field_724; bool sub_0047a460(); };
typedef char offset_check[offsetof(Object0047a460, field_724) == 0x724 ? 1 : -1];
bool Object0047a460::sub_0047a460() { return field_724 != 0; }
