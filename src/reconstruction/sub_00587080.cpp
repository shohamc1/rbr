struct Object00587080 {
    unsigned char unknown_00[4];
    unsigned int field_04;
    unsigned int sub_00587080(unsigned int index);
};
typedef char field_offset_assert[(sizeof(Object00587080) == 8) ? 1 : -1];
unsigned int Object00587080::sub_00587080(unsigned int index) { return index * 0x46c + (unsigned int)&field_04; }
