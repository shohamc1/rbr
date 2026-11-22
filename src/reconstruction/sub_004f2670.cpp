#include "layout.h"
struct Object004f2670 {
    unsigned char unknown_00[0xc]; unsigned int field_0c;
    unsigned int sub_004f2670();
};
LAYOUT_OFFSET(Object004f2670, field_0c, 12);

// FUNCTION: RBR 0x004f2670
unsigned int Object004f2670::sub_004f2670() { return field_0c; }
