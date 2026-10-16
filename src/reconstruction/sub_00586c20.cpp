struct Object00586c20 {
    unsigned char unknown_00[0x424];
    unsigned int storage_base_424;
    void sub_00586c20(unsigned int index, unsigned int value);
};
static_assert(__builtin_offsetof(Object00586c20, storage_base_424) == 0x424, "storage base offset");
void Object00586c20::sub_00586c20(unsigned int index, unsigned int value) { unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0x424); words[index] = value; }
