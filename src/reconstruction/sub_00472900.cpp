// sub_00472900: EXE 0x00472900, 76 bytes. __thiscall with a float and an int
// stack word (RET 8). Compiled with MSVC 7.1.
//
// Container of 0xc-byte records: dword count at +0, record base at +4; each
// record holds a type dword at +0 and a float at +8. Scans from the last
// record (index count - 1) down to index 0 and returns the first record whose
// float field is less than `value` and whose type is outside [0x15, 0x17]
// (any type when `flag` is zero). Returns 0 when the container is empty or
// nothing matches. Reached through the thunk at 0x00473810, which passes the
// container at outer_this + 0x10 + 0x20. Names are hypotheses.
#include "layout.h"

struct Record00472900 {
    unsigned int type;
    unsigned char unknown_04[4];
    float field_08;
};

struct Object00472900 {
    unsigned int count;             // +0
    Record00472900* base;           // +4
    unsigned char unknown_08[4];

    Record00472900* sub_00472900(float value, int flag);
};

LAYOUT_OFFSET(Object00472900, base, 0x4);
LAYOUT_OFFSET(Record00472900, field_08, 0x8);
LAYOUT_SIZE(Record00472900, 0xc);

Record00472900* Object00472900::sub_00472900(float value, int flag) {
    int i = count - 1;
    if (i >= 0) {
        do {
            if (base[i].field_08 < value
                && (flag == 0 || base[i].type < 0x15 || base[i].type > 0x17))
                return base + i;
        } while (--i >= 0);
    }
    return 0;
}
