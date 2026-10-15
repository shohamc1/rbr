struct Object00401000 {
    unsigned char unknown_00[12];
    unsigned int field_0c;
    unsigned int sub_00401000();
};
typedef char FieldOffsetCheck[sizeof(Object00401000) == 16 ? 1 : -1];
unsigned int Object00401000::sub_00401000() { return field_0c; }
