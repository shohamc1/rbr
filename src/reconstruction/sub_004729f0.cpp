// sub_004729f0: EXE 0x004729f0, 28 bytes. __thiscall with no stack words,
// returns a dword. Compiled with MSVC 7.1.
//
// Maps the dword at +0x4 through an unsigned range chain: values 0xfffffff0
// and 0xfffffff1 map to 0xfffffff0, the value 0xfffffff2 is returned
// unchanged (the fall-through RET keeps the loaded field), and everything
// else maps to 0. Callers at 0x004836a3, 0x004867fc, 0x004ab602, 0x004ce1df
// and 0x005ce236. Field purpose unknown; the value looks like a two's-
// complement enum. Names are hypotheses.
#include "layout.h"

struct Object004729f0 {
    unsigned char unknown_00[4];
    unsigned int field_04;

    unsigned int sub_004729f0();
};

LAYOUT_OFFSET(Object004729f0, field_04, 0x4);

unsigned int Object004729f0::sub_004729f0() {
    unsigned int value = field_04;
    switch (value) {
    case 0xfffffff0:
    case 0xfffffff1:
        return 0xfffffff0u;
    case 0xfffffff2:
        return value;
    }
    return 0;
}
