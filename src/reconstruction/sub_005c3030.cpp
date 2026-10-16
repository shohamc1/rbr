struct Object005c3030 {
    unsigned char unknown_00[0xa8];
    unsigned int storage_base_a8;
    void sub_005c3030(unsigned int index, unsigned int value);
};
static_assert(__builtin_offsetof(Object005c3030, storage_base_a8) == 0xa8, "storage base offset");
void Object005c3030::sub_005c3030(unsigned int index, unsigned int value) { unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0xa8); words[index] = value; }
