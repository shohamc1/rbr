// Object00533f00: a state object reached through the global at 0x007aeb78.
//
// Merge evidence: EXE code loads the same global as `this` for all four
// methods. Function 0x0047d810 calls sub_005342a0 (0x0047d996) and then
// sub_005341e0 with argument 5 (0x0047d9b0) on it. Other sites: sub_00533f00
// (0x0047f156, 0x0047f1f7, 0x0047f356) and sub_00534390 (0x0047b965,
// 0x0047de85, 0x0048449a, 0x0056386f). The field at +8 is a state word:
// sub_005341e0 stores it and sub_005342a0 and sub_00534390 compare it with 0
// and 5. The compiler profile differs per method, so this header builds
// under both Clang and MSVC 7.1. Names are hypotheses.
#pragma once

#include "layout.h"

struct Object00533f00 {
    unsigned char unknown_00[8];
    unsigned int field_8;       // state word
    unsigned int field_c;

    void sub_00533f00(unsigned int value);
    void sub_005341e0(unsigned int value);
    unsigned int sub_005342a0();
    unsigned int sub_00534390();
};

LAYOUT_OFFSET(Object00533f00, field_8, 8);
LAYOUT_OFFSET(Object00533f00, field_c, 0xc);
