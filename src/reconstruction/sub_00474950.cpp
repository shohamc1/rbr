// sub_00474950: EXE 0x00474950, 106 bytes. __thiscall with no stack words,
// returns 0/1. Compiled with MSVC 7.1.
//
// State check on the object fed by the global at 0x007ca200. Guards: the
// pointer at +0x10 must be set, the state dword at +0xe8 must equal 1, the
// key float of slot 0 (+0x10c) must equal the zero float at 0x00776a70, and
// the flag bits 0-1 of the byte at +0xf8 must be clear; otherwise returns 1.
// The tail compares the floats at +0xfc, +0xec, +0xf4 and +0xf0 against the
// same zero constant. The vtable data reference at 0x0070f988 marks this as
// a virtual method; the definition here is the direct body. Callers:
// 0x004767f5, 0x0056fb7d, 0x0054eb99, 0x005a1eb4, 0x0047fcf4.
// Field purposes unknown. Names are hypotheses.
//
// The negated `!(field_ec >= field_f4)` is load-bearing: it selects the
// TEST AH,1 / JNE encoding (unordered counts as less) and keeps the shared
// return-0 block as the fall-through; a plain `<` compiles to TEST AH,5/JNP.
#include "layout.h"

struct Object00474950 {
    unsigned char unknown_00[0x10];
    void* field_10;                 // +0x10, tested for zero
    unsigned char unknown_14[0xd4];
    unsigned int field_e8;          // +0xe8, compared with 1
    float field_ec;
    float field_f0;
    float field_f4;
    unsigned char field_f8;         // +0xf8, flag bits 0-1 tested
    unsigned char unknown_f9[3];
    float field_fc;
    unsigned char unknown_100[0xc];
    float field_10c;                // +0x10c, slot 0 key

    int sub_00474950();
};

LAYOUT_OFFSET(Object00474950, field_10, 0x10);
LAYOUT_OFFSET(Object00474950, field_e8, 0xe8);
LAYOUT_OFFSET(Object00474950, field_ec, 0xec);
LAYOUT_OFFSET(Object00474950, field_f8, 0xf8);
LAYOUT_OFFSET(Object00474950, field_fc, 0xfc);
LAYOUT_OFFSET(Object00474950, field_10c, 0x10c);

int Object00474950::sub_00474950() {
    if (field_10 == 0)
        return 0;
    if (field_e8 != 1)
        return 1;
    if (field_10c != *(float *)0x00776a70)
        return 1;
    if (field_f8 & 3)
        return 1;
    if (field_fc > *(float *)0x00776a70) {
        if (!(field_ec >= field_f4))
            return 1;
        return 0;
    }
    if (field_ec <= field_f0)
        return 0;
    return 1;
}
