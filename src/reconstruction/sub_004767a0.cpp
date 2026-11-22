// sub_004767a0: EXE 0x004767a0, 103 bytes. __thiscall with four stack words
// (RET 0x10). Compiled with MSVC 7.1.
//
// Dispatcher on the object fed by the global at 0x007ca200: sets the state
// dword at +0xe8 to 1, forwards the arguments to sub_00473e70 on the slot
// array at +0x100 with the third and fourth arguments swapped, counts the
// slots whose key float still equals the zero float at 0x00776a70, and when
// exactly one slot is free calls sub_00474950 and, on a zero result,
// sub_00476270. The reference issues one MOV ECX,ESI for the sub_00474950
// call and relies on it preserving ECX for the sub_00476270 call that
// follows. Callers: 0x0047868c, 0x0056fbe1. Names are hypotheses.
#include "layout.h"

// Slot-array methods on the receiver at +0x100 of this object.
class Object00473e70 {
public:
    int sub_00473e70(unsigned int value0, unsigned int value1,
                     unsigned int value2, float key);
};

class Object00474950 {
public:
    int sub_00474950();
};

// Called here without an ECX setup: the reference issues one MOV ECX,ESI for
// sub_00474950 and relies on it preserving ECX, so this call site must leave
// ECX alone (same register-input pattern as sub_0067f514).
extern "C" void __cdecl sub_00476270();

struct Slot004767a0 {
    unsigned char unknown_00[0xc];
    float key;                      // +0xc
};

struct Object004767a0 {
    unsigned char unknown_00[0xe8];
    unsigned int field_e8;          // +0xe8, state dword
    unsigned char unknown_ec[0x14];
    Slot004767a0 field_100[4];      // +0x100, slot array

    void sub_004767a0(unsigned int value0, unsigned int value1,
                      float value2, unsigned int value3);
};

LAYOUT_OFFSET(Object004767a0, field_e8, 0xe8);
LAYOUT_OFFSET(Object004767a0, field_100, 0x100);
LAYOUT_OFFSET(Slot004767a0, key, 0xc);

void Object004767a0::sub_004767a0(unsigned int value0, unsigned int value1,
                                  float value2, unsigned int value3) {
    field_e8 = 1;
    ((Object00473e70*)field_100)->sub_00473e70(value0, value1, value3, value2);
    // Count the slots whose key still equals the zero float; the swap of
    // value2/value3 in the forwarding call is the reference's own argument
    // order (its third and fourth stack words land exchanged at 0x00473e70).
    unsigned int n = 0;
    do {
        if (field_100[n].key == 0.0f)
            break;
        ++n;
    } while (n < 4);
    if (n == 1) {
        if (((Object00474950*)this)->sub_00474950() == 0)
            sub_00476270();
    }
}
