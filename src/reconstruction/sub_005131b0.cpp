struct Object005131b0 {
    unsigned char unknown_00[1408];
    unsigned int* field_580;
    unsigned int sub_005131b0();
};
typedef char FieldOffsetCheck[sizeof(Object005131b0) == 1412 ? 1 : -1];
unsigned int Object005131b0::sub_005131b0() { return *field_580 == 2; }
