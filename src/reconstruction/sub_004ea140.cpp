// sub_004ea140: EXE 0x004ea140, 10 bytes. __thiscall member with one stack
// argument (RET 4): loads the argument and stores it at this+4.
// Callers: 0x00493b64 (sub_00493b40), 0x0059ce52 (sub_0059cdf0).
struct Object004ea140 {
    unsigned int field_00;
    unsigned int field_04;

    void sub_004ea140(unsigned int value);
};

#include "layout.h"
LAYOUT_OFFSET(Object004ea140, field_04, 0x04);

void Object004ea140::sub_004ea140(unsigned int value)
{
    field_04 = value;
}
