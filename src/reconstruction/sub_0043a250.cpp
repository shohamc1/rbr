// sub_0043a250: EXE 0x0043a250, 11 bytes. Compiled with MSVC 7.1 (/O2 /Zl).
//
// thiscall cursor store: writes the stack argument to the pointer at +0 and
// returns the receiver (MOV EAX,ECX entry home; EAX is this at RET 4).
// Pairs with sub_0043a210 (advance, also returning the receiver) and
// sub_0043a260 (difference); +0 is the cursor over 4-byte elements. No EXE
// caller known; the convention comes from ECX input with one stack argument
// (RET 4).
#include "layout.h"

class Object0043a250 {
public:
    Object0043a250* sub_0043a250(unsigned int* pointer);
    unsigned int* field_0;           // cursor over 4-byte elements
};

LAYOUT_OFFSET(Object0043a250, field_0, 0x0);

Object0043a250* Object0043a250::sub_0043a250(unsigned int* pointer) {
    field_0 = pointer;
    return this;
}
