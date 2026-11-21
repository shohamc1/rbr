// sub_004728c0: EXE 0x004728c0, 59 bytes. __thiscall with two stack words
// (RET 8): a record cursor and a type-filter flag. Compiled with MSVC 7.1.
//
// Container of 0xc-byte records: dword count at +0, record base at +4; each
// record holds a type dword at +0. Scans forward from the record strictly
// after `cursor` up to and including the last record (base + count - 1); the
// first record whose type is outside [0x15, 0x17] — or any record when the
// incoming flag is zero — is returned. Returns 0 for a null cursor, when the
// cursor already sits on the last record, or at the end of the chain.
//
// Callers reach it through the thunks at 0x004737f0 and 0x00474410/0x004744f0/
// 0x00474850, which pass the container at outer_this + 0x10 + 0x20 with the
// previous scan result as the cursor. Names are hypotheses.
//
// The do/while (flag) tail is load-bearing: flag was just set to 1, so the
// condition folds away and MSVC keeps the leading exit test at the loop head
// with a plain JMP back edge. A while (cursor != last) loop gets inverted
// into an entry test plus a duplicated bottom compare.
#include "layout.h"

struct Record004728c0 {
    unsigned int type;
    unsigned char unknown_04[8];
};

struct Object004728c0 {
    unsigned int count;             // +0
    Record004728c0* base;           // +4
    unsigned char unknown_08[4];

    Record004728c0* sub_004728c0(Record004728c0* cursor, int flag);
};

LAYOUT_OFFSET(Object004728c0, base, 0x4);
LAYOUT_SIZE(Record004728c0, 0xc);

Record004728c0* Object004728c0::sub_004728c0(Record004728c0* cursor, int flag) {
    if (cursor != 0) {
        do {
            if (cursor == base + count - 1)
                return 0;
            ++cursor;
            if (flag == 0 || cursor->type < 0x15 || cursor->type > 0x17)
                return cursor;
            flag = 1;
        } while (flag);
    }
    return 0;
}
