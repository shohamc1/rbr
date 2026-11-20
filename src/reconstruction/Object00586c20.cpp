// Object00586c20: the element type of the array that sub_00587070 and
// sub_00587080 (Object00587070 at +0x4, stride 0x46c) address.
//
// Merge evidence: sub_00587070 returns the address of that array. In function
// 0x00557d10 EAX from its return value becomes `this` for sub_00586c70
// (0x00558290, 0x005582fe, 0x0055839a) and for sub_00586c20 (0x00558316,
// 0x005583bc). Both methods index the word array at +0x424. Names are
// hypotheses.
struct Object00586c20 {
    unsigned char unknown_00[0x424];
    unsigned int storage_base_424;

    void sub_00586c20(unsigned int index, unsigned int value);
    unsigned int sub_00586c70(unsigned int index);
};
static_assert(__builtin_offsetof(Object00586c20, storage_base_424) == 0x424, "storage base offset");

void Object00586c20::sub_00586c20(unsigned int index, unsigned int value) {
    unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0x424);
    words[index] = value;
}

unsigned int Object00586c20::sub_00586c70(unsigned int index) {
    unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0x424);
    return words[index];
}
