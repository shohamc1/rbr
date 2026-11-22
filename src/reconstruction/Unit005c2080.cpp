// Object005c2080: __thiscall, ECX in and EAX = this
// out: a constructor (or an equivalent reset member). Six dword stores:
// +0x0, +0x4, +0x8 = -1 (single OR ECX,-1 value reused three times),
// +0xc, +0x14 = 1 (MOV ECX,1 reused), +0x10 = 0 immediate last.
// Called from sub_005c1840 at 0x005c1bc4 and 0x005c1c94.
//
// Encoding lever: MSVC 7.1 /O2 keeps the stores in statement order here, so
// the +0x14 assignment must precede the +0x10 assignment in the body; the
// declaration-order body interleaves the 0 immediate between the two
// ECX=1 stores ([eax+0xc], [eax+0x10]=0, [eax+0x14]) and misses by one
// instruction swap.

struct Object005c2080 {
    unsigned int field_00;   // -1
    unsigned int field_04;   // -1
    unsigned int field_08;   // -1
    unsigned int field_0c;   // 1
    unsigned int field_10;   // 0
    unsigned int field_14;   // 1

    Object005c2080();
};

#include "layout.h"
// Object005c2f90 is the 0xe8-byte element type of the container at 0x007b5a04.
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
LAYOUT_OFFSET(Object005c2f90, field_84, 0x84);
LAYOUT_OFFSET(Object005c2f90, field_8c, 0x8c);
LAYOUT_OFFSET(Object005c2f90, storage_base_98, 0x98);
LAYOUT_OFFSET(Object005c2f90, storage_base_a8, 0xa8);
LAYOUT_SIZE(Object005c2f90, 0xe8);

#include "layout.h"
struct Object005c3890 {
    unsigned int field_00;
    unsigned int sub_005c3890();
};
LAYOUT_OFFSET(Object005c3890, field_00, 0);

#include "layout.h"
struct Object005c38a0 {
    unsigned char unknown_00[0x50];
    unsigned int storage_base_50;
    unsigned int sub_005c38a0(unsigned int index);
};
LAYOUT_OFFSET(Object005c38a0, storage_base_50, 0x50);

#include "layout.h"
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
LAYOUT_OFFSET(Object005c3900, field_00, 0);
LAYOUT_OFFSET(Object005c3900, field_04, 4);
LAYOUT_OFFSET(Object005c3900, storage_base_50, 0x50);

// FUNCTION: RBR 0x005c2080
Object005c2080::Object005c2080() {
    field_00 = -1;
    field_04 = -1;
    field_08 = -1;
    field_0c = 1;
    field_14 = 1;
    field_10 = 0;
}

// FUNCTION: RBR 0x005c2f90
unsigned int Object005c2f90::sub_005c2f90() { return field_8c; }

// FUNCTION: RBR 0x005c2fe0
void Object005c2f90::sub_005c2fe0() { field_84 = 0; }

// FUNCTION: RBR 0x005c3000
void Object005c2f90::sub_005c3000(unsigned int index, unsigned int value) {
    unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0x98);
    words[index] = value;
}

// FUNCTION: RBR 0x005c3020
unsigned int Object005c2f90::sub_005c3020(unsigned int index) {
    unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0xa8);
    return words[index];
}

// FUNCTION: RBR 0x005c3030
void Object005c2f90::sub_005c3030(unsigned int index, unsigned int value) {
    unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0xa8);
    words[index] = value;
}

// FUNCTION: RBR 0x005c3890
unsigned int Object005c3890::sub_005c3890() { return field_00; }

// FUNCTION: RBR 0x005c38a0
unsigned int Object005c38a0::sub_005c38a0(unsigned int index) { unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0x50); return words[index]; }

// FUNCTION: RBR 0x005c3900
void Object005c3900::sub_005c3900(unsigned int value) { field_00 = value; }

// FUNCTION: RBR 0x005c3910
void Object005c3900::sub_005c3910(unsigned int index, unsigned int value) {
    unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0x50);
    words[index] = value;
}

// FUNCTION: RBR 0x005c3970
void Object005c3900::sub_005c3970(unsigned int value) { field_04 = value; }
