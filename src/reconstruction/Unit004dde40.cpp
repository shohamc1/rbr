#include "layout.h"
// Object004dde40: one object, reached through the global at 0x008cecc0.
//
// Merge evidence: EXE code loads the same global as `this` for sub_004dde40
// (0x00534acf, 0x00534ae2, 0x00534aed, 0x00534b53) and for sub_004dde90
// (0x0052be8e). The field offsets do not overlap. Names are hypotheses.
struct Object004dde40 {
    unsigned char unknown_00[0x34];
    unsigned int field_34;
    unsigned char unknown_38[0xad4];
    float field_b0c;

    unsigned int sub_004dde40();
    float sub_004dde90();
};
LAYOUT_OFFSET(Object004dde40, field_34, 0x34);
LAYOUT_OFFSET(Object004dde40, field_b0c, 0xb0c);

#include "layout.h"
struct Object004dde60 {
    unsigned char unknown_00[0xa00]; unsigned char field_a00;
    unsigned char * sub_004dde60();
};
LAYOUT_OFFSET(Object004dde60, field_a00, 2560);

#include "layout.h"
struct Object004ddea0 {
    unsigned char unknown_00[0xb1c]; float field_b1c;
    float sub_004ddea0();
};
LAYOUT_OFFSET(Object004ddea0, field_b1c, 2844);

#include "layout.h"
struct Object004de000 {
    unsigned char unknown_00[0x15f8]; float field_15f8;
    float sub_004de000();
};
LAYOUT_OFFSET(Object004de000, field_15f8, 5624);

#include "layout.h"
struct Object004de070 {
    unsigned char unknown_00[0x30]; unsigned int field_30;
    unsigned int sub_004de070();
};
LAYOUT_OFFSET(Object004de070, field_30, 48);

// FUNCTION: RBR 0x004dde40
unsigned int Object004dde40::sub_004dde40() { return field_34; }

// FUNCTION: RBR 0x004dde60
unsigned char * Object004dde60::sub_004dde60() { return &field_a00; }

// FUNCTION: RBR 0x004dde90
float Object004dde40::sub_004dde90() { return field_b0c; }

// FUNCTION: RBR 0x004ddea0
float Object004ddea0::sub_004ddea0() { return field_b1c; }

// FUNCTION: RBR 0x004de000
float Object004de000::sub_004de000() { return field_15f8; }

// FUNCTION: RBR 0x004de070
unsigned int Object004de070::sub_004de070() { return field_30; }
