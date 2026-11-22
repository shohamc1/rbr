// sub_004c4170: EXE 0x004c4170, 9 bytes. MSVC 7.1 (/O2 /Zl).
//
// Destructor of the container element class matched as Object004c4170: the
// class embeds the 0x1c-byte MSVCP71 basic_string at offset +8 and its
// destructor only destroys that member, so MSVC emits
// `add ecx,8; jmp [__imp_??1?$basic_string@...]` (IAT slot 0x0070c2d8).
// The scalar destroy helper sub_004c1ef0 and eleven other container members
// call this destructor. The string import declaration is in msvcp71.h.

#include "msvcp71.h"

class Object004c4170 {
public:
    unsigned char unknown_00[8];
    std::string field_08;
    ~Object004c4170();
};

Object004c4170::~Object004c4170() {
}
