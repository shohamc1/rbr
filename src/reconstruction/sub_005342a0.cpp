struct Object005342a0 {
    unsigned char unknown_00[8];
    unsigned int field_8;
    unsigned int sub_005342a0();
};
typedef char FieldOffsetCheck[sizeof(Object005342a0) == 12 ? 1 : -1];
unsigned int Object005342a0::sub_005342a0() { return field_8 == 0; }
