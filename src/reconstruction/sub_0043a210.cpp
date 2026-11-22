// sub_0043a210: EXE 0x0043a210, 18 bytes. Compiled with MSVC 7.1 (/O2 /Zl).
//
// thiscall cursor advance: adds count elements (LEA with scale 4, memory
// ADD) to the pointer at +0 and returns the receiver (MOV EAX,ECX entry
// home; EAX is this at RET 4). Pairs with sub_0043a250, which stores the
// same pointer and also returns the receiver, and sub_0043a260 (pointer
// difference as an element count); +0 is a cursor over 4-byte elements.
// No EXE caller known; the convention comes from ECX input with one stack
// argument (RET 4).
#include "layout.h"

class Object0043a210 {
public:
    Object0043a210* sub_0043a210(unsigned int count);
    unsigned int* field_0;           // cursor over 4-byte elements
};

LAYOUT_OFFSET(Object0043a210, field_0, 0x0);

Object0043a210* Object0043a210::sub_0043a210(unsigned int count) {
    field_0 += count;
    return this;
}
