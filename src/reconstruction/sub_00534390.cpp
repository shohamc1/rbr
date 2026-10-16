struct Object00534390 {
    unsigned char unknown_00[8];
    unsigned int field_8;
    unsigned int sub_00534390();
};
typedef char FieldOffsetCheck[sizeof(Object00534390) == 12 ? 1 : -1];
unsigned int Object00534390::sub_00534390() { return field_8 == 5; }
