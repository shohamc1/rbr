// sub_004729a0: EXE 0x004729a0, 57 bytes. __thiscall with two stack words
// (RET 8): a record cursor and a type-filter flag. Compiled with MSVC 7.1.
//
// Container of 0xc-byte records: record base at +4 (the count at +0 is not
// read); each record holds a type dword at +0. Scans backwards from `cursor`
// while the cursor stays above `base`; when the current record's type is
// outside [0x15, 0x17] — or for any record when the incoming flag is zero —
// returns the record one step before the cursor. Returns 0 for a null
// cursor or when the scan reaches the base. Reached through the thunk at
// 0x00473830, which passes the container at outer_this + 0x10 + 0x20.
// Names are hypotheses.
#include "layout.h"

struct Record004729a0 {
    unsigned int type;
    unsigned char unknown_04[8];
};

struct Object004729a0 {
    unsigned char unknown_00[4];
    Record004729a0* base;           // +4
    unsigned char unknown_08[4];

    Record004729a0* sub_004729a0(Record004729a0* cursor, int flag);
};

LAYOUT_OFFSET(Object004729a0, base, 0x4);
LAYOUT_SIZE(Record004729a0, 0xc);

Record004729a0* Object004729a0::sub_004729a0(Record004729a0* cursor, int flag) {
    if (cursor != 0) {
        do {
            if (cursor == base)
                return 0;
            if (flag == 0 || cursor->type < 0x15 || cursor->type > 0x17)
                return cursor - 1;
            --cursor;
            flag = 1;
        } while (cursor != 0);
    }
    return 0;
}
