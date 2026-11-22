// Object004181e0: a reader state object (at least 0x18 bytes) that function 0x0046cc20
// builds on its stack.
//
// Merge evidence: that function constructs the object at [esp+4] with
// sub_004181e0 (0x0046d030) and then calls sub_004185a0 (0x0046d0c4) and
// sub_00418590 (0x0046d0f4, 0x0046d10f) on the same stack slot, with ESP
// adjusted for the pushes in between. sub_004181e0 zeroes field_04, the word
// the other two read and write. The compiler profile differs per method, so
// this header builds under MSVC 7.1. Names are hypotheses.
#pragma once

#include "layout.h"

struct Object004181e0 {
    unsigned int field_00, field_04;
    unsigned char unknown_08[4];
    unsigned char field_0c;
    unsigned char unknown_0d[3];
    unsigned int field_10, field_14;

    Object004181e0();
    void sub_00418590(unsigned int value);
    unsigned int sub_004185a0();
};

LAYOUT_OFFSET(Object004181e0, field_04, 4);
LAYOUT_OFFSET(Object004181e0, field_0c, 12);
LAYOUT_OFFSET(Object004181e0, field_14, 20);
