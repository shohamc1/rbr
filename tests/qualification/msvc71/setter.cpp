struct Object004068e0 {
    unsigned char unknown_00[60];
    unsigned int field_3c;
    void sub_004068e0(unsigned int value);
};
typedef char FieldOffsetCheck[sizeof(Object004068e0) == 64 ? 1 : -1];
void Object004068e0::sub_004068e0(unsigned int value) { field_3c = value; }
