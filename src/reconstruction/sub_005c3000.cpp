struct Object005c3000 {
    unsigned char unknown_00[0x98];
    unsigned int storage_base_98;
    void sub_005c3000(unsigned int index, unsigned int value);
};
static_assert(__builtin_offsetof(Object005c3000, storage_base_98) == 0x98, "storage base offset");
void Object005c3000::sub_005c3000(unsigned int index, unsigned int value) { unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0x98); words[index] = value; }
