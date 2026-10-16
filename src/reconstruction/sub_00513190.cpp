struct Object00513190 {
    unsigned char unknown_00[1408];
    unsigned int* field_580;
    unsigned int sub_00513190();
};
typedef char FieldOffsetCheck[sizeof(Object00513190) == 1412 ? 1 : -1];
unsigned int Object00513190::sub_00513190() { return *field_580 == 1; }
