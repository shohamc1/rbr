#include "layout.h"
struct Object004f85f0 {
    unsigned char unknown_00[8]; unsigned int field_08;
    unsigned int sub_004f85f0();
};
LAYOUT_OFFSET(Object004f85f0, field_08, 8);

// FUNCTION: RBR 0x004f85f0
unsigned int Object004f85f0::sub_004f85f0() { return field_08; }
