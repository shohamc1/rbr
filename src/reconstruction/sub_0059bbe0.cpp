#include "layout.h"
struct Object0059bbe0 {
    unsigned char unknown_00[4];
    unsigned int field_04;
    void sub_0059bbe0(unsigned int value);
};
LAYOUT_OFFSET(Object0059bbe0, field_04, 4);

// FUNCTION: RBR 0x0059bbe0
void Object0059bbe0::sub_0059bbe0(unsigned int value) { field_04 = value; }
