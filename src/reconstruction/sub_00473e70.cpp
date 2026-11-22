// sub_00473e70: EXE 0x00473e70, 80 bytes. __thiscall with four stack words
// (RET 0x10). Compiled with MSVC 7.1.
//
// The receiver is an array of four 0x10-byte slots at +0: three payload
// dwords at +0/+4/+8 and a float at +0xc used as the occupancy key. Finds
// the first slot whose key equals the zero float at 0x00776a70, fills all
// four fields (the fourth argument lands in the key) and returns 1; returns
// 0 when every slot is occupied. The sub_004767a0 dispatcher calls it on the
// slot array at outer receiver + 0x100 after swapping its third and fourth
// arguments. Names are hypotheses.
#include "layout.h"

struct Slot00473e70 {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    float key;                      // +0xc, compared with 0.0f
};

struct Object00473e70 {
    Slot00473e70 slots[4];

    int sub_00473e70(unsigned int value0, unsigned int value1,
                     unsigned int value2, float key);
};

LAYOUT_OFFSET(Slot00473e70, key, 0xc);
LAYOUT_SIZE(Slot00473e70, 0x10);
LAYOUT_SIZE(Object00473e70, 0x40);

int Object00473e70::sub_00473e70(unsigned int value0, unsigned int value1,
                                 unsigned int value2, float key) {
    for (unsigned int i = 0; i < 4; i++) {
        if (slots[i].key == *(float *)0x00776a70) {
            slots[i].field_0 = value0;
            slots[i].field_4 = value1;
            slots[i].field_8 = value2;
            slots[i].key = key;
            return 1;
        }
    }
    return 0;
}
