#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
// The entry returns this in EAX while it clears two words and one short.
struct Object00492020 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned short field_08;
    Object00492020();
};
typedef char offset_check[offsetof(Object00492020, field_08) == 8 ? 1 : -1];
Object00492020::Object00492020() { field_00 = 0; field_04 = 0; field_08 = 0; }
