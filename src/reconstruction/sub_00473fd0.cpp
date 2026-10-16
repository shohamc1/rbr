#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
// The entry returns this in EAX while it clears three fields.
struct Object00473fd0 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    Object00473fd0();
};
typedef char offset_check[offsetof(Object00473fd0, field_08) == 8 ? 1 : -1];
Object00473fd0::Object00473fd0() { field_00 = 0; field_04 = 0; field_08 = 0; }
