// sub_0040e960: EXE 0x0040e960, 38 bytes. MSVC 7.1 (/O2 /Zl).
//
// Object0040e570-family member (inner record at +0x6e4; see
// Object0040e570.h). thiscall, no stack arguments, bare RET.
//
// Sets the inner change flags at +0x8 and +0x9, then, through the global
// pointer at 0x016e304c (beside the FILE* global at 0x016e3044), loads
// the pointer at +8 of the pointee and zeroes the dwords at +0x2c and
// +0x30 of that record. These are the same flags sub_0040e6a0 and
// sub_0040e6f0 test and clear.
#include "layout.h"

// Pointee of the +0x6e4 inner pointer for this member.
struct Inner0040e960 {
    unsigned char unknown_00[8];
    unsigned char field_08;
    unsigned char field_09;
};

LAYOUT_OFFSET(Inner0040e960, field_08, 0x08);
LAYOUT_OFFSET(Inner0040e960, field_09, 0x09);

// Pointee of the global at 0x016e304c.
struct Target0040e960;

struct Holder0040e960 {
    unsigned char unknown_00[8];
    Target0040e960* field_08;
};

LAYOUT_OFFSET(Holder0040e960, field_08, 0x08);

// Pointee of Holder0040e960::field_08.
struct Target0040e960 {
    unsigned char unknown_00[0x2c];
    unsigned int field_2c;
    unsigned int field_30;
};

LAYOUT_OFFSET(Target0040e960, field_2c, 0x2c);
LAYOUT_OFFSET(Target0040e960, field_30, 0x30);

extern "C" Holder0040e960* gHolder016e304c; // EXE .data 0x016e304c, beside the FILE* at 0x016e3044

class Object0040e960 {
public:
    unsigned char unknown_00[0x6e4];
    Inner0040e960* field_6e4;

    void sub_0040e960();
};

LAYOUT_OFFSET(Object0040e960, field_6e4, 0x6e4);

// NONMATCHING (one instruction position): the chained flag assignment
// reproduces every instruction and encoding, including the EDX inner load,
// the shared `mov al,1`, the ECX reload, and the 8b 15 global load, but the
// reference hoists `mov edx,[0x016e304c]` above the second flag store
// (`mov [ecx+9],al`); this /O2 build emits it one instruction later. The
// hoist needs a source shape that both sequences the load between the
// reload and the store and keeps the flag value flowing to the second
// store (which is what pins the constant in AL and the inner load in EDX).
// Plain two-statement forms sequence the load correctly but recolor to
// eax/dl/a1; see analysis/notes.md.
void Object0040e960::sub_0040e960() {
    field_6e4->field_09 = (field_6e4->field_08 = 1);
    Holder0040e960* holder = gHolder016e304c;
    Target0040e960* target = holder->field_08;
    target->field_2c = 0;
    target->field_30 = 0;
}
