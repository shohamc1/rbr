// Object005c2f90: the 0xe8-byte element type of the container at 0x007b5a04.
//
// Merge evidence: sub_005b5550 returns `[this+0x318] + index*0xe8` for that
// container, and sub_005bc990 tail-calls it. Function 0x005bd720 passes the
// returned element as `this` to sub_005c3000 (0x005bd75e) and sub_005c3030
// (0x005bd8a0, 0x005bd8c8). Function 0x005a9ae0 keeps the element from
// sub_005bc990 in EBX and passes it to sub_005c2f90 (0x005a9c7a) and
// sub_005c3020 (0x005a9caa). Function 0x005b65d0 loads the stack slot
// [esp+0x50] as `this` for sub_005c2fe0 (0x005b6600) and sub_005c2f90
// (0x005b663c).
// The word arrays at +0x98 and +0xa8 are indexed without a known bound. Names
// are hypotheses.
struct Object005c2f90 {
    unsigned char unknown_00[0x84];
    unsigned int field_84;
    unsigned char unknown_88[4];
    unsigned int field_8c;
    unsigned char unknown_90[8];
    unsigned int storage_base_98;
    unsigned char unknown_9c[0x0c];
    unsigned int storage_base_a8;
    unsigned char unknown_ac[0x3c];

    unsigned int sub_005c2f90();
    void sub_005c2fe0();
    void sub_005c3000(unsigned int index, unsigned int value);
    unsigned int sub_005c3020(unsigned int index);
    void sub_005c3030(unsigned int index, unsigned int value);
};
static_assert(__builtin_offsetof(Object005c2f90, field_84) == 0x84, "field offset");
static_assert(__builtin_offsetof(Object005c2f90, field_8c) == 0x8c, "field offset");
static_assert(__builtin_offsetof(Object005c2f90, storage_base_98) == 0x98, "storage base offset");
static_assert(__builtin_offsetof(Object005c2f90, storage_base_a8) == 0xa8, "storage base offset");
static_assert(sizeof(Object005c2f90) == 0xe8, "element size");

unsigned int Object005c2f90::sub_005c2f90() { return field_8c; }
void Object005c2f90::sub_005c2fe0() { field_84 = 0; }

void Object005c2f90::sub_005c3000(unsigned int index, unsigned int value) {
    unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0x98);
    words[index] = value;
}

unsigned int Object005c2f90::sub_005c3020(unsigned int index) {
    unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0xa8);
    return words[index];
}

void Object005c2f90::sub_005c3030(unsigned int index, unsigned int value) {
    unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0xa8);
    words[index] = value;
}
