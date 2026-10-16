#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
// The entry returns this in EAX while it clears four fields.
struct Object0048cb70 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
    Object0048cb70();
};
typedef char offset_check[offsetof(Object0048cb70, field_0c) == 0xc ? 1 : -1];
Object0048cb70::Object0048cb70() { field_08 = 0; field_00 = 0; field_04 = 0; field_0c = 0; }
