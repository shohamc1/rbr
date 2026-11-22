// sub_0043a140: EXE 0x0043a140, 19 bytes. Compiled with MSVC 7.1 (/O2 /Zl).
//
// thiscall accessor of the same vector-like container family as
// sub_0043a120, but the difference runs from +4 (first) to +0xc (storage
// end): the allocated element count. Returns 0 when the container is empty.
// No EXE caller or address reference is known yet; the convention comes from
// the ECX-only register input.
#include "layout.h"

class Object0043a140 {
public:
    int sub_0043a140();
    unsigned char unknown_00[0x4];
    unsigned int* field_4;         // first element
    unsigned char unknown_08[0x4];
    unsigned int* field_c;         // storage end
};

LAYOUT_OFFSET(Object0043a140, field_4, 0x4);
LAYOUT_OFFSET(Object0043a140, field_c, 0xc);

int Object0043a140::sub_0043a140() {
    if (field_4 == 0) {
        return 0;
    }
    return field_c - field_4;
}
