// Object00587070: an object reached through the global at 0x007b1e8c that
// holds an array of 0x46c-byte elements from offset 0x4.
//
// Merge evidence: EXE code loads the same global as `this` for sub_00587070
// (81 call sites, for example 0x00534be6), sub_00587080 (17, for example
// 0x0054f071) and sub_005877e0 (12, for example 0x0058d62b). Function
// 0x00555c60 calls sub_00587080 (0x00555cb1) and sub_00587070 (0x00555d03) on
// it. sub_00587070 returns the array base, and the returned pointer is the
// `this` of Object00586c20 methods (see Unit00586c20.cpp). sub_00587830, called with the same ECX at 0x0058d79c, is a free
// function and stays outside the class. Names are hypotheses.
#pragma once

#include "layout.h"

struct Object00587070 {
    unsigned char unknown_00[4];
    unsigned int field_04;          // first array element; stride 0x46c
    unsigned char unknown_08[0x11b4];
    unsigned int field_11bc;

    unsigned int* sub_00587070();
    unsigned int sub_00587080(unsigned int index);
    unsigned int sub_005877e0();
};

LAYOUT_OFFSET(Object00587070, field_04, 4);
LAYOUT_OFFSET(Object00587070, field_11bc, 0x11bc);
