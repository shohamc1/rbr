// Object0040e570: the object that the application global at 0x007c9fe8 holds at
// offset 0x28 (sub_00471110 reads it as `application->window`).
//
// Merge evidence for one class: EXE code loads `[[0x007c9fe8]+0x28]` as `this`
// for each method below. Function 0x004bb9c0 does this for sub_0040e740
// (0x004bba22), sub_0040e750 (0x004bba35), sub_0040e900 (0x004bbaa3),
// sub_0040e920 (0x004bbb5b) and sub_0040e930 (0x004bbb6b). Other sites:
// sub_0040e570 (0x004714e7), sub_0040df40 (0x00448db6, 30 sites in all),
// sub_0040e9e0 (0x004490c1), sub_0040ea00 (0x00448da3), sub_0040ea30
// (0x004b9fc6) and sub_0040eb90 (0x00533f2d). The field offsets do not
// overlap, and the pointer at +0x6e4 is shared by sub_0040ea00 and
// sub_0040eb90. The compiler profile differs per method, so this header builds
// under MSVC 7.1.
//
// Names are hypotheses. sub_00471110 uses sub_0040e570 as a window handle
// source for SetForegroundWindow and SetFocus; the rest are unnamed.
#pragma once

#include "layout.h"

// Pointee of the pointer at Object0040e570+0x6e4. sub_0040ea00 zeroes fields
// 0x30 to 0x4c and 0x64 to 0x74; sub_0040eb90 reads the byte at +0x10.
struct Inner0040ea00 {
    unsigned char unknown_00[0x10];
    unsigned char field_10;
    unsigned char unknown_11[0x1f];
    unsigned int field_30;
    unsigned int field_34;
    unsigned int field_38;
    unsigned int field_3c;
    unsigned int field_40;
    unsigned int field_44;
    unsigned int field_48;
    unsigned int field_4c;
    unsigned char unknown_50[0x10];
    unsigned int field_60;
    unsigned int field_64;
    unsigned int field_68;
    unsigned int field_6c;
    unsigned int field_70;
    unsigned int field_74;
};

LAYOUT_OFFSET(Inner0040ea00, field_10, 0x10);
LAYOUT_OFFSET(Inner0040ea00, field_74, 0x74);

struct Object0040e570 {
    unsigned char unknown_00[0xa0];
    unsigned char field_a0;          // sub_0040ea30
    unsigned char unknown_a1[0x43];
    unsigned int field_e4;           // sub_0040e570
    unsigned char unknown_e8[0x0c];
    unsigned int field_f4;           // sub_0040df40
    unsigned char unknown_f8[0x148];
    unsigned int field_240;          // sub_0040e920
    unsigned int field_244;          // sub_0040e930
    unsigned char unknown_248[0x8e];
    unsigned char field_2d6;         // sub_0040e9e0 returns its address
    unsigned char unknown_2d7[0x3e9];
    unsigned int field_6c0[8];       // sub_0040e900 copies eight dwords in
    unsigned char field_6e0;         // sub_0040e740
    unsigned char field_6e1;         // sub_0040e750
    unsigned char unknown_6e2[2];
    Inner0040ea00* inner;            // +0x6e4, sub_0040ea00 and sub_0040eb90

    unsigned int sub_0040df40();
    unsigned int sub_0040e570();
    void sub_0040e740(unsigned char value);
    void sub_0040e750(unsigned char value);
    void sub_0040e900(const unsigned int* values);
    unsigned int sub_0040e920();
    unsigned int sub_0040e930();
    unsigned char* sub_0040e9e0();
    void sub_0040ea00();
    unsigned int sub_0040ea30();
    unsigned char sub_0040eb90();
};

LAYOUT_OFFSET(Object0040e570, field_a0, 0xa0);
LAYOUT_OFFSET(Object0040e570, field_e4, 0xe4);
LAYOUT_OFFSET(Object0040e570, field_f4, 0xf4);
LAYOUT_OFFSET(Object0040e570, field_240, 0x240);
LAYOUT_OFFSET(Object0040e570, field_244, 0x244);
LAYOUT_OFFSET(Object0040e570, field_2d6, 0x2d6);
LAYOUT_OFFSET(Object0040e570, field_6c0, 0x6c0);
LAYOUT_OFFSET(Object0040e570, field_6e0, 0x6e0);
LAYOUT_OFFSET(Object0040e570, field_6e1, 0x6e1);
LAYOUT_OFFSET(Object0040e570, inner, 0x6e4);
