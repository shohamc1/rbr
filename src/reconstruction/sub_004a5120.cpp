// sub_004a5120: EXE 0x004a5120, 74 bytes. MSVC 7.1 (/O2 /Zl).
//
// thiscall member, two stack arguments (float at [esp+4], unsigned index at
// [esp+8]), RET 8. Writes the value into the two-float array at +0x24 when
// the index is below 2, then restarts a controller: current (+0x2c) seeded
// from +0x24 by an integer register copy, velocity (+0x30) and the state
// words +0x34/+0x38 zeroed (with +0x34 zeroed a second time after its read
// and +0x38 then set to 300.0f), current advanced by the zero-valued product
// with the velocity, and the velocity set to the distance from the target at
// +0x28 scaled by the float constant 1/300 at 0x00714514.
//
// All five float words are volatile: every reference read and write goes
// back to memory (integer-register seed, the zero product re-loaded and
// re-stored, +0x2c re-read after its store), and the read of +0x34 sits
// between its two zero stores. Plain members let /O2 fold the product away
// and fuse the seed into the final x87 subtraction.
//
// NONMATCHING (residual on the two +0x2c reads as binary-op operands): the
// accumulate compiles to `fld [ecx+0x2c]; fadd st(1); fstp st(0)` where the
// reference has `fadd [ecx+0x2c]` — MSVC 7.1 gives a volatile operand an
// explicit FLD when the other operand is a named local (the split product),
// and only keeps the memory form when the other operand is an arithmetic
// subexpression or a volatile leaf. The velocity subtraction has the right
// memory form (`fsub [ecx+0x2c]`, direct +0x28 leaf) but its
// `fld [ecx+0x28]` is scheduled at the subtraction instead of the
// reference's early preload above `fld [ecx+0x34]`. Local experiments that
// fix one side break the other: an in-expression product restores
// `fadd [ecx+0x2c]` but turns the multiply into
// `fld [ecx+0x30]; fmul st(1)` and hoists the +0x38=300 store above the
// multiply; a non-volatile +0x2c keeps the memory forms but lets the
// optimizer forward the stored sum (`fst; fxch; fsub st(1)`); volatile-cast
// reads pop the store yet re-FLD the value. /Op, /Og-, /O1, /Os, and
// #pragma optimize("s") all degrade the zero stores or add spills.

class Object004a5120 {
public:
    unsigned char field_00[0x24];
    volatile float field_24[2]; // +0x24 start value, +0x28 target value
    volatile float field_2c;    // current
    volatile float field_30;    // velocity
    volatile float field_34;    // state
    volatile float field_38;    // state
    void sub_004a5120(float value, unsigned int index);
};

void Object004a5120::sub_004a5120(float value, unsigned int index)
{
    if (index < 2)
        field_24[index] = value;
    field_2c = field_24[0];
    field_30 = 0.0f;
    field_34 = 0.0f;
    field_38 = 0.0f;
    float delta = field_34;
    field_34 = 0.0f;
    delta = delta * field_30;
    field_38 = 300.0f;
    field_2c = delta + field_2c;
    field_30 = (field_24[1] - field_2c) * (1.0f / 300.0f);
}
