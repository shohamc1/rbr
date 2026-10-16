struct Object00406a20 {
    unsigned int field_00;
    unsigned char unknown_04[0x14];
    unsigned int field_18, field_1c, field_20;
    unsigned char field_24, field_25, field_26, field_27, field_28, field_29, field_2a;
    unsigned char unknown_2b[9];
    unsigned char field_34;
    unsigned char unknown_35[3];
    unsigned int field_38;
    unsigned char field_3c;
    Object00406a20();
};
typedef char object_field_18[(unsigned int)&(((Object00406a20*)0)->field_18) == 0x18 ? 1 : -1];
typedef char object_field_34[(unsigned int)&(((Object00406a20*)0)->field_34) == 0x34 ? 1 : -1];
typedef char object_field_38[(unsigned int)&(((Object00406a20*)0)->field_38) == 0x38 ? 1 : -1];
Object00406a20::Object00406a20() {
    field_18 = 4;
    field_1c = 4;
    field_00 = 0;
    field_20 = 8;
    field_24 = 1; field_25 = 1; field_26 = 1; field_27 = 1;
    field_28 = 0; field_29 = 1; field_2a = 1;
    field_34 = 0; field_38 = 0; field_3c = 0;
}
