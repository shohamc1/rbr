struct Object005c38a0 {
    unsigned char unknown_00[0x50];
    unsigned int storage_base_50;
    unsigned int sub_005c38a0(unsigned int index);
};
static_assert(__builtin_offsetof(Object005c38a0, storage_base_50) == 0x50, "storage base offset");
unsigned int Object005c38a0::sub_005c38a0(unsigned int index) { unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0x50); return words[index]; }
