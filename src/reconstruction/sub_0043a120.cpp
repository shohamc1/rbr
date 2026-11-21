// sub_0043a120: EXE 0x0043a120, 19 bytes. Compiled with MSVC 7.1 (/O2 /Zl).
//
// thiscall accessor of a vector-like container whose element pointers sit at
// +4 (first) and +8 (last). Returns 0 when the container is empty, otherwise
// the element count (last - first with 4-byte elements: SUB then SAR 2).
// The storage-end sibling at +0xc belongs to sub_0043a140. No EXE caller or
// address reference is known yet; the convention comes from the ECX-only
// register input.
#include "layout.h"

class Object0043a120 {
public:
    int sub_0043a120();
    unsigned char unknown_00[0x4];
    unsigned int* field_4;         // first element
    unsigned int* field_8;         // last element
};

LAYOUT_OFFSET(Object0043a120, field_4, 0x4);
LAYOUT_OFFSET(Object0043a120, field_8, 0x8);

int Object0043a120::sub_0043a120() {
    if (field_4 == 0) {
        return 0;
    }
    return field_8 - field_4;
}
