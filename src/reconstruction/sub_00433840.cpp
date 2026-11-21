// Reset helper for the object family created in sub_00433890: the caller at
// 0x00433928 tail-jumps here with the outer object in EDX (`mov edx,esi;
// pop ebx; jmp 0x00433840`), a custom register input outside the cdecl
// manifest declaration. Compiled as __fastcall with an unused first argument
// so the object arrives in EDX; the ECX register input is dead in the body.
//
// The outer object (EDX) owns an inner object at +0x1c; the inner object
// holds a byte pool at +0x528 whose address is stored into the three list
// heads at +0x44, +0x48, and +0x64 (reset to an empty state, matching the
// allocator pattern in sub_00433890, which allocates 0x1ba8 bytes for it).
// Returns 0 on success, -2 when either object pointer is null.
// Compiled with MSVC 7.1 (/O2 /Zl).
#include "layout.h"

struct Object00433840Inner {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_c;
    unsigned int field_10;
    unsigned int field_14;
    unsigned int field_18;
    unsigned int field_1c;
    unsigned int field_20;
    unsigned int field_24;
    unsigned int field_28;
    unsigned int field_2c;
    unsigned int field_30;
    unsigned int field_34;
    unsigned char field_38[0x0c];
    void *field_44;
    void *field_48;
    unsigned char field_4c[0x18];
    void *field_64;
    unsigned char field_68[0x4c0];
    unsigned char field_528;
};
typedef char Assert00433840Inner44[((unsigned int)&(((Object00433840Inner*)0)->field_44) == 0x44) ? 1 : -1];
typedef char Assert00433840Inner528[((unsigned int)&(((Object00433840Inner*)0)->field_528) == 0x528) ? 1 : -1];

struct Object00433840Outer {
    unsigned char field_0[0x8];
    unsigned int field_8;
    unsigned char field_c[0x8];
    unsigned int field_14;
    unsigned int field_18;
    Object00433840Inner *field_1c;
};
typedef char Assert00433840Outer1c[((unsigned int)&(((Object00433840Outer*)0)->field_1c) == 0x1c) ? 1 : -1];

extern "C" int __fastcall sub_00433840(void *unused, Object00433840Outer *outer) {
    if (outer == 0 || outer->field_1c == 0) {
        return -2;
    }
    Object00433840Inner *inner = outer->field_1c;
    inner->field_18 = 0;
    outer->field_14 = 0;
    outer->field_8 = 0;
    outer->field_18 = 0;
    inner->field_0 = 0;
    inner->field_4 = 0;
    inner->field_c = 0;
    inner->field_20 = 0;
    inner->field_24 = 0;
    inner->field_30 = 0;
    inner->field_34 = 0;
    void *pool = &inner->field_528;
    inner->field_64 = pool;
    inner->field_48 = pool;
    inner->field_44 = pool;
    return 0;
}
