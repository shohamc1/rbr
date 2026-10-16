struct Object004c4a70 {
    unsigned char unknown_00[4]; unsigned char *field_04;
    unsigned char * sub_004c4a70();
};
typedef char FieldOffsetCheck[sizeof(Object004c4a70) == 8 ? 1 : -1];
unsigned char * Object004c4a70::sub_004c4a70() { return field_04 + 0x1c8; }
