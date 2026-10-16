struct Object005c3910 {
    unsigned char unknown_00[0x50];
    unsigned int storage_base_50;
    void sub_005c3910(unsigned int index, unsigned int value);
};
static_assert(__builtin_offsetof(Object005c3910, storage_base_50) == 0x50, "storage base offset");
void Object005c3910::sub_005c3910(unsigned int index, unsigned int value) { unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0x50); words[index] = value; }
