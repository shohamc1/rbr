// sub_0043a260: EXE 0x0043a260, 14 bytes. Compiled with MSVC 7.1 (/O2 /Zl).
//
// thiscall cursor difference: (this->cursor - other->cursor) as a 4-byte
// element count (SUB with a memory operand, then SAR 2). The argument is
// dereferenced, so it is the same cursor-holding record, not a raw pointer.
// Pairs with sub_0043a210/sub_0043a250. No EXE caller known; the convention
// comes from ECX input with one stack argument (RET 4).
#include "layout.h"

class Object0043a260 {
public:
    int sub_0043a260(Object0043a260* other);
    unsigned int* field_0;           // cursor over 4-byte elements
};

LAYOUT_OFFSET(Object0043a260, field_0, 0x0);

int Object0043a260::sub_0043a260(Object0043a260* other) {
    return field_0 - other->field_0;
}
