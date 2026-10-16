struct Object005c3020 {
    unsigned char unknown_00[0xa8];
    unsigned int storage_base_a8;
    unsigned int sub_005c3020(unsigned int index);
};
static_assert(__builtin_offsetof(Object005c3020, storage_base_a8) == 0xa8, "storage base offset");
unsigned int Object005c3020::sub_005c3020(unsigned int index) { unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0xa8); return words[index]; }
