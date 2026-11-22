#include "layout.h"
struct Object00520320 {
    unsigned char unknown_00[8];
    unsigned int field_8;
    void sub_00520320();
};
LAYOUT_OFFSET(Object00520320, field_8, 8);

// FUNCTION: RBR 0x00520320
void Object00520320::sub_00520320() { field_8 = 1; }
