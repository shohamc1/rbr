// sub_00420c40: EXE 0x00420c40, 9 bytes. MSVC 7.1 (/O2 /Zl).
//
// Destructor thunk identical in shape to sub_00420c30: the class embeds the
// 0x1c-byte MSVCP71 basic_string at offset +4 (four bytes of plain data
// before it), so MSVC emits `add ecx,4; jmp [__imp_??1?$basic_string@...]`
// (IAT slot 0x0070c2d8). Reached as a member destructor at 0x00705574 and
// 0x00705584, next to the sub_00420c30 call sites. The string import
// declaration is in msvcp71.h.

#include "msvcp71.h"

class Object00420c40 {
public:
    unsigned char unknown_00[4];
    std::string field_04;
    ~Object00420c40();
};

Object00420c40::~Object00420c40() {
}
