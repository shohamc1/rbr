#include "layout.h"
struct Object00523140 {
    unsigned char unknown_00[16];
    unsigned int field_10;
    unsigned int sub_00523140();
};
LAYOUT_OFFSET(Object00523140, field_10, 16);

#include "layout.h"
struct Object00523150 {
    unsigned char unknown_00[20];
    unsigned int field_14;
    unsigned int sub_00523150();
};
LAYOUT_OFFSET(Object00523150, field_14, 20);

// FUNCTION: RBR 0x00523140
unsigned int Object00523140::sub_00523140() { return field_10; }

// FUNCTION: RBR 0x00523150
unsigned int Object00523150::sub_00523150() { return field_14; }
