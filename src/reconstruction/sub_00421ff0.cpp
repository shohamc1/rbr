// sub_00421ff0: EXE 0x00421ff0, 9 bytes. MSVC 7.1 (/O2 /Zl).
//
// Destructor thunk of the tree node class of the sub_004213b0 hierarchical
// list: the node keeps 0x10 bytes of links (sibling chain +0, backlink +4,
// child chain +8) before the 0x1c-byte MSVCP71 basic_string at +0x10, and
// its destructor only destroys that string, so MSVC emits
// `add ecx,0x10; jmp [__imp_??1?$basic_string@...]` (IAT slot 0x0070c2d8).
// The byte at +0x2d right after the string is the sentinel flag tested by
// sub_004213b0/sub_00421840/sub_004218a0/sub_00421a10/sub_00421de0. Called
// from sub_004213b0 at 0x004213d1. The string import declaration is in
// msvcp71.h.

#include "msvcp71.h"

class Object00421ff0 {
public:
    unsigned char unknown_00[0x10];
    std::string field_10;
    ~Object00421ff0();
};

Object00421ff0::~Object00421ff0() {
}
