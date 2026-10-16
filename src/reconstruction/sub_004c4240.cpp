struct Object004c4240 {
    unsigned char unknown_00[44]; unsigned int field_2c;
    int sub_004c4240();
};
typedef char FieldOffsetCheck[sizeof(Object004c4240) == 48 ? 1 : -1];
int Object004c4240::sub_004c4240() { return field_2c != 0; }
