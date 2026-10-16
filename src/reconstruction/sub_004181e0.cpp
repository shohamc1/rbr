struct Object004181e0 {
    unsigned int field_00, field_04;
    unsigned char unknown_08[4];
    unsigned char field_0c;
    unsigned char unknown_0d[3];
    unsigned int field_10, field_14;
    Object004181e0();
};
#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
typedef char field_0c_offset_check[offsetof(Object004181e0, field_0c) == 12 ? 1 : -1];
typedef char field_14_offset_check[offsetof(Object004181e0, field_14) == 20 ? 1 : -1];
Object004181e0::Object004181e0() { field_00 = 0; field_04 = 0; field_0c = 0; field_10 = 0; field_14 = 0; }
