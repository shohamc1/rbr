// Object005c3900: the element type of the array that function 0x005b70e0
// reaches through the pointer at offset 0x304 of its object.
//
// Merge evidence: that function computes `[obj+0x304] + offset` as `this` for
// sub_005c3900 (0x005b799b), sub_005c3970 (0x005b7a13) and sub_005c3910
// (0x005b7c6c). Names are hypotheses.
struct Object005c3900 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned char unknown_08[0x48];
    unsigned int storage_base_50;

    void sub_005c3900(unsigned int value);
    void sub_005c3910(unsigned int index, unsigned int value);
    void sub_005c3970(unsigned int value);
};
static_assert(__builtin_offsetof(Object005c3900, field_00) == 0, "field offset");
static_assert(__builtin_offsetof(Object005c3900, field_04) == 4, "field offset");
static_assert(__builtin_offsetof(Object005c3900, storage_base_50) == 0x50, "storage base offset");

void Object005c3900::sub_005c3900(unsigned int value) { field_00 = value; }

void Object005c3900::sub_005c3910(unsigned int index, unsigned int value) {
    unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0x50);
    words[index] = value;
}

void Object005c3900::sub_005c3970(unsigned int value) { field_04 = value; }
