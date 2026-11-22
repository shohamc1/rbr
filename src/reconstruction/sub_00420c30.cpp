// sub_00420c30: EXE 0x00420c30, 9 bytes. MSVC 7.1 (/O2 /Zl).
//
// Destructor thunk of a class whose only member needing destruction is the
// 0x1c-byte MSVCP71 basic_string embedded at offset +4 (four bytes of plain
// data before it). MSVC emits `add ecx,4; jmp [__imp_??1?$basic_string@...]`
// (IAT slot 0x0070c2d8), the same shape as the matched sub_004c4170 thunk
// (string at +8 there). Reached as a member destructor at 0x00704b5c,
// 0x0070556c and 0x0070557c. The string import declaration is in msvcp71.h.

#include "msvcp71.h"

class Object00420c30 {
public:
    unsigned char unknown_00[4];
    std::string field_04;
    ~Object00420c30();
};

Object00420c30::~Object00420c30() {
}
