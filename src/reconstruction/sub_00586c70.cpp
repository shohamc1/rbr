struct Object00586c70 {
    unsigned char unknown_00[0x424];
    unsigned int storage_base_424;
    unsigned int sub_00586c70(unsigned int index);
};
static_assert(__builtin_offsetof(Object00586c70, storage_base_424) == 0x424, "storage base offset");
unsigned int Object00586c70::sub_00586c70(unsigned int index) { unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0x424); return words[index]; }
