// sub_004ee120: __stdcall with three pointer-sized
// stack arguments (RET 0xc): returns 1. No direct callers are known; the
// shape fits a boolean dispatch-table slot that ignores its inputs.

#include "layout.h"
struct Object004eee30 {
    unsigned char unknown_00[0x67c]; unsigned int field_67c;
    unsigned int sub_004eee30();
};
LAYOUT_OFFSET(Object004eee30, field_67c, 1660);

// FUNCTION: RBR 0x004ee120
extern "C" int __stdcall sub_004ee120(void *receiver, void *argument0, void *argument1)
{
    (void)receiver;
    (void)argument0;
    (void)argument1;
    return 1;
}

// FUNCTION: RBR 0x004eee30
unsigned int Object004eee30::sub_004eee30() { return field_67c; }
