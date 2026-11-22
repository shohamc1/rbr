// sub_00421ff0: MSVC 7.1 (/O2 /Zl).
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

// sub_00422180: MSVC 7.1 (/O2 /Zl).
//
// Stride-or-table index resolver: __fastcall with the record in ECX and the
// index in EDX (mangled @sub_00422180@8). When the mode word at +0 is zero,
// the record is (0, base +4, stride +8) and the result is stride*index +
// base; otherwise the dword at +4 is a pointer table and the result is
// table[index]. sub_00423d50 inlines this exact shape twice (0x00423f56 and
// 0x00423f66 region) for records reached through the container at 0x016e3050.
// Called from sub_00422730 at 0x00422938 and 0x004229ed.

// FUNCTION: RBR 0x00421ff0
Object00421ff0::~Object00421ff0() {
}

// FUNCTION: RBR 0x00422180
extern "C" int __fastcall sub_00422180(void* self, int index) {
    if (*(int*)self == 0) {
        return ((int*)self)[2] * index + ((int*)self)[1];
    }
    return ((int**)self)[1][index];
}
