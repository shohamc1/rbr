extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)

struct Object004066c0 {
    unsigned int field_00, field_04, field_08, field_0c, field_10;
    Object004066c0();
};
#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
typedef char field_offset_check[offsetof(Object004066c0, field_10) == 16 ? 1 : -1];

#include "layout.h"
struct Object004068e0 {
    unsigned char unknown_00[60];
    unsigned int field_3c;
    void sub_004068e0(unsigned int value);
};
LAYOUT_OFFSET(Object004068e0, field_3c, 60);

#include "layout.h"
struct Object00406900 { unsigned char unknown_00[0x28]; unsigned int field_28; void sub_00406900(unsigned int); };
LAYOUT_OFFSET(Object00406900, field_28, 0x28);

#include "layout.h"
struct Object00406940 { unsigned char unknown_00[0x74]; unsigned int field_74; void sub_00406940(unsigned int); };
LAYOUT_OFFSET(Object00406940, field_74, 0x74);

// sub_00406970: MSVC 7.1 (/O2 /Zl).
//
// Indexed setter: the receiver holds an array of 0x74-byte records at +0x10
// (the Collection00416390/004163c0 record size; see analysis/notes.md), and
// this writes the last dword of the selected record:
//   field_10[index].field_70 = value;
// thiscall, two stack arguments, RET 8. No callers have source yet.

struct Object00406970Record {
    unsigned char body[0x70];
    unsigned int field_70;
};

struct Object00406970 {
    unsigned char field_00[0x10];
    Object00406970Record* field_10;
    void sub_00406970(unsigned int index, unsigned int value);
};

struct Object00406a20 {
    unsigned int field_00;
    unsigned char unknown_04[0x14];
    unsigned int field_18, field_1c, field_20;
    unsigned char field_24, field_25, field_26, field_27, field_28, field_29, field_2a;
    unsigned char unknown_2b[9];
    unsigned char field_34;
    unsigned char unknown_35[3];
    unsigned int field_38;
    unsigned char field_3c;
    Object00406a20();
};
typedef char object_field_18[(unsigned int)&(((Object00406a20*)0)->field_18) == 0x18 ? 1 : -1];
typedef char object_field_34[(unsigned int)&(((Object00406a20*)0)->field_34) == 0x34 ? 1 : -1];
typedef char object_field_38[(unsigned int)&(((Object00406a20*)0)->field_38) == 0x38 ? 1 : -1];

// FUNCTION: RBR 0x00406490
extern "C" float sub_00406490(float value) { return (float)fabs(value); }

// FUNCTION: RBR 0x004066c0
Object004066c0::Object004066c0() { field_00 = 0; field_04 = 0; field_08 = 0; field_0c = 0; field_10 = 0; }

// FUNCTION: RBR 0x004068e0
void Object004068e0::sub_004068e0(unsigned int value) { field_3c = value; }

// FUNCTION: RBR 0x00406900
void Object00406900::sub_00406900(unsigned int value) { field_28 = value; }

// FUNCTION: RBR 0x00406940
void Object00406940::sub_00406940(unsigned int value) { field_74 = value; }

// FUNCTION: RBR 0x00406970
void Object00406970::sub_00406970(unsigned int index, unsigned int value) {
    field_10[index].field_70 = value;
}

// FUNCTION: RBR 0x00406a20
Object00406a20::Object00406a20() {
    field_18 = 4;
    field_1c = 4;
    field_00 = 0;
    field_20 = 8;
    field_24 = 1; field_25 = 1; field_26 = 1; field_27 = 1;
    field_28 = 0; field_29 = 1; field_2a = 1;
    field_34 = 0; field_38 = 0; field_3c = 0;
}
