// sub_004729e0: EXE 0x004729e0, 10 bytes. __thiscall with no stack words.
// Compiled with MSVC 7.1.
//
// Clears the dword at +0x10 and returns the receiver in EAX
// (MOV EAX,ECX precedes the store). No EXE references reach it; it is most
// likely a table entry. Field purpose unknown. Names are hypotheses.
#include "layout.h"

struct Object004729e0 {
    unsigned char unknown_00[0x10];
    unsigned int field_10;

    Object004729e0* sub_004729e0();
};

LAYOUT_OFFSET(Object004729e0, field_10, 0x10);

Object004729e0* Object004729e0::sub_004729e0() {
    field_10 = 0;
    return this;
}
