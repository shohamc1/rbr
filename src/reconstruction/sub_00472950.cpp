// sub_00472950: EXE 0x00472950, 76 bytes. __thiscall with a float and an int
// stack word (RET 8). Compiled with MSVC 7.1.
//
// Byte-identical twin of sub_00472900 at a different address; no EXE call
// site reaches it directly, so it most likely sits in an as-yet-unmapped
// dispatch table. Container of 0xc-byte records (count at +0, base at +4,
// type dword at +0, float at +8). Scans from the last record down to index 0
// and returns the first record whose float field is less than `value` and
// whose type is outside [0x15, 0x17] (any type when `flag` is zero).
// Returns 0 when the container is empty or nothing matches. Names are
// hypotheses.
#include "layout.h"

struct Record00472950 {
    unsigned int type;
    unsigned char unknown_04[4];
    float field_08;
};

struct Object00472950 {
    unsigned int count;             // +0
    Record00472950* base;           // +4
    unsigned char unknown_08[4];

    Record00472950* sub_00472950(float value, int flag);
};

LAYOUT_OFFSET(Object00472950, base, 0x4);
LAYOUT_OFFSET(Record00472950, field_08, 0x8);
LAYOUT_SIZE(Record00472950, 0xc);

Record00472950* Object00472950::sub_00472950(float value, int flag) {
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
