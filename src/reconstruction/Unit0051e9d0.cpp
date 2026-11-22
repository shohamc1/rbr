#include "layout.h"
struct Object0051e9d0 {
    unsigned char unknown_00[4];
    unsigned int field_4;
    void sub_0051e9d0();
};
LAYOUT_OFFSET(Object0051e9d0, field_4, 4);

#include "layout.h"
struct Object0051eab0 {
    unsigned char unknown_00[16];
    unsigned int field_10;
    void sub_0051eab0(unsigned int value);
};
LAYOUT_OFFSET(Object0051eab0, field_10, 16);

#include "layout.h"
struct Object0051eb70 {
    unsigned char unknown_00[4];
    unsigned int field_4;
    unsigned int sub_0051eb70();
};
LAYOUT_OFFSET(Object0051eb70, field_4, 4);

// FUNCTION: RBR 0x0051e9d0
void Object0051e9d0::sub_0051e9d0() { field_4 = 0; }

// FUNCTION: RBR 0x0051eab0
void Object0051eab0::sub_0051eab0(unsigned int value) { field_10 = value; }

// FUNCTION: RBR 0x0051eb70
unsigned int Object0051eb70::sub_0051eb70() { return field_4; }
