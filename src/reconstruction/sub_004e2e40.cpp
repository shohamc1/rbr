#include "layout.h"
struct Object004e2e40 {
    unsigned char unknown_00[0x34]; unsigned int field_34;
    unsigned int sub_004e2e40();
};
LAYOUT_OFFSET(Object004e2e40, field_34, 52);

// FUNCTION: RBR 0x004e2e40
unsigned int Object004e2e40::sub_004e2e40() { return field_34; }
