// sub_004c4170: MSVC 7.1 (/O2 /Zl).
//
// Destructor of the container element class matched as Object004c4170: the
// class embeds the 0x1c-byte MSVCP71 basic_string at offset +8 and its
// destructor only destroys that member, so MSVC emits
// `add ecx,8; jmp [__imp_??1?$basic_string@...]` (IAT slot 0x0070c2d8).
// The scalar destroy helper sub_004c1ef0 and eleven other container members
// call this destructor. The string import declaration is in msvcp71.h.

#include "msvcp71.h"

class Object004c4170 {
public:
    unsigned char unknown_00[8];
    std::string field_08;
    ~Object004c4170();
};

#include "layout.h"
struct Object004c4180 {
    unsigned char unknown_00[8]; unsigned char field_08;
    unsigned char * sub_004c4180();
};
LAYOUT_OFFSET(Object004c4180, field_08, 8);

#include "layout.h"
struct Object004c41c0 {
    unsigned char unknown_00[48]; float field_30;
    float sub_004c41c0();
};
LAYOUT_OFFSET(Object004c41c0, field_30, 48);

struct Object004c4240 {
    unsigned char unknown_00[44]; unsigned int field_2c;
    int sub_004c4240();
};
typedef char FieldOffsetCheck[sizeof(Object004c4240) == 48 ? 1 : -1];

struct Object004c4a70 {
    unsigned char unknown_00[4]; unsigned char *field_04;
    unsigned char * sub_004c4a70();
};
typedef char FieldOffsetCheck[sizeof(Object004c4a70) == 8 ? 1 : -1];

#include "layout.h"
struct Object004c4b00 {
    unsigned char unknown_00[4]; unsigned char *field_04;
    unsigned char * sub_004c4b00();
};
LAYOUT_OFFSET(Object004c4b00, field_04, 4);

#include "layout.h"
struct Child004c4b20 {
    unsigned char unknown_00[0x214];
    unsigned int field_214;
};
struct Object004c4b20 {
    unsigned char unknown_00[4]; Child004c4b20 *field_04;
    unsigned int sub_004c4b20();
};
LAYOUT_OFFSET(Object004c4b20, field_04, 4);
LAYOUT_OFFSET(Child004c4b20, field_214, 0x214);

// FUNCTION: RBR 0x004c4170
Object004c4170::~Object004c4170() {
}

// FUNCTION: RBR 0x004c4180
unsigned char * Object004c4180::sub_004c4180() { return &field_08; }

// FUNCTION: RBR 0x004c41c0
float Object004c41c0::sub_004c41c0() { return field_30; }

// FUNCTION: RBR 0x004c4240
int Object004c4240::sub_004c4240() { return field_2c != 0; }

// FUNCTION: RBR 0x004c4a70
unsigned char * Object004c4a70::sub_004c4a70() { return field_04 + 0x1c8; }

// FUNCTION: RBR 0x004c4b00
unsigned char * Object004c4b00::sub_004c4b00() { return field_04 + 0x24; }

// FUNCTION: RBR 0x004c4b20
unsigned int Object004c4b20::sub_004c4b20() { return field_04->field_214; }
