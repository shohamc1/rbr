// sub_004ea1f0: EXE 0x004ea1f0, 74 bytes. Constructor, __thiscall, no
// stack arguments, ECX-in/EAX-out (this homed in ESI, returned in EAX).
//
// Builds the 3x3 identity in three 16-byte-stride float rows at +0x60,
// +0x70, +0x80 (x/y/z stored, w at +0x6c/+0x7c/+0x8c untouched), then
// re-runs the sub-object constructor sub_00463ff0 (EXE 0x00463ff0,
// manifest ??0Object00463ff0@@QAE@XZ) at this+0, and finally sets the
// float trio at +0x20 (1.0f, 2.2f, 0.75f). 1.0f is pooled in EDI
// (0x3f800000), 0.0f in EAX (XOR), single-use constants are stored as
// immediates. The call reuses the incoming ECX home of this with no
// reload, via the MSVC explicit-constructor-call extension.
//
// NONMATCHING (score 4, 74 vs 74 bytes): every instruction matches except
// the placement of PUSH EDI + MOV EDI,0x3f800000 — /O2 hoists both above
// the two zero stores; the reference materializes 1.0f just-in-time after
// them. Source order swaps (x/y/z, y/z/x), a pooled local, chained
// assignment, and optimize("s"/"g"/"t"/"") all fail (the "s" form flips
// the stores to x87). Same scheduling-only class as the SIMD-family
// residuals, without the family's homing slot.
class Object00463ff0 {
public:
    Object00463ff0();  // EXE 0x00463ff0
};

struct Object004ea1f0 {
    unsigned char field_00[0x20];
    float field_20;
    float field_24;
    float field_28;
    unsigned char field_2c[0x34];
    float row[3][4];

    Object004ea1f0();
};

#include "layout.h"
LAYOUT_OFFSET(Object004ea1f0, field_20, 0x20);
LAYOUT_OFFSET(Object004ea1f0, row, 0x60);

Object004ea1f0::Object004ea1f0()
{
    row[0][1] = 0.0f;
    row[0][2] = 0.0f;
    row[0][0] = 1.0f;
    row[1][0] = 0.0f;
    row[1][1] = 1.0f;
    row[1][2] = 0.0f;
    row[2][0] = 0.0f;
    row[2][1] = 0.0f;
    row[2][2] = 1.0f;
    ((Object00463ff0 *)this)->Object00463ff0::Object00463ff0();
    field_20 = 1.0f;
    field_24 = 2.2f;
    field_28 = 0.75f;
}
