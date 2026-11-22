// sub_004ea140: __thiscall member with one stack
// argument (RET 4): loads the argument and stores it at this+4.
// Callers: 0x00493b64 (sub_00493b40), 0x0059ce52 (sub_0059cdf0).
struct Object004ea140 {
    unsigned int field_00;
    unsigned int field_04;

    void sub_004ea140(unsigned int value);
};

#include "layout.h"
LAYOUT_OFFSET(Object004ea140, field_04, 0x04);

// sub_004ea160: Scalar deleting destructor of the
// class whose vtable is 0x0071be8c: __thiscall, one stack argument (the
// destroy/delete flags, RET 4), returns this in EAX.
// Homes this in ESI, calls the destructor body sub_004ea0b0 (EXE 0x004ea0b0,
// stub in config/symbols.csv; seeds vtable 0x71be8c and frees the 256 pointer
// fields at this+8..this+0x404 through sub_006efdd0) without resetting ECX
// to this — the call relies on the incoming ECX home, so the declaration
// stays cdecl. On flag bit 0 frees this through the cdecl operator-delete
// thunk sub_006efdd0 (EXE 0x006efdd0) with add-esp cleanup.
// Vtable slot 0 of 0x0071be8c (referenced by data at 0x0071be8c).
extern "C" void __cdecl sub_004ea0b0();
extern "C" void __cdecl sub_006efdd0(void *memory);

class Object004ea160 {
public:
    void *sub_004ea160(unsigned int flags);
};

// sub_004ea180: Default constructor, __thiscall,
// no parameters, ECX-in/EAX-out (MOV EAX,ECX prologue).
// Seeds field_00 with the vtable candidate at EXE 0x0071be94 (slot 0 =
// sub_004ea1b0, the deleting destructor; COL at 0x0071be90), following the
// raw-vtable idiom of the matched sub_004995c0 (vtable 0x007137b4).

extern "C" const void* const gVtable0071be94[]; // EXE .rdata 0x0071be94

class Object004ea180 {
public:
    Object004ea180();

private:
    unsigned int vtable;
};

// sub_004ea1a0: __stdcall with one pointer-sized
// stack argument (RET 4): returns 1. No direct callers are known; the shape
// fits a boolean dispatch-table slot that ignores its receiver.

// sub_004ea1b0: Scalar deleting destructor of the
// class whose vtable is 0x0071be94: __thiscall, one stack argument (the
// destroy/delete flags, RET 4), returns this in EAX.
// Tests flag bit 0 (delete) before homing this, re-seeds the vtable, and on
// the delete path frees this through the cdecl operator-delete thunk
// sub_006efdd0 (EXE 0x006efdd0) with add-esp cleanup. Vtable slot 0 of
// 0x0071be94; the matching default constructor is sub_004ea180.

extern "C" const void* const gVtable0071be94[]; // EXE .rdata 0x0071be94
extern "C" void __cdecl sub_006efdd0(void *memory);

class Object004ea1b0 {
public:
    void *sub_004ea1b0(unsigned int flags);

private:
    unsigned int vtable;
};

// sub_004ea1e0: __thiscall member, no stack
// arguments: returns (this[0x68a] >> 2) & 1 as a full EAX value.
// MSVC 7.1 widens the unsigned char with XOR EAX,EAX + MOV AL (not MOVZX).
class Object004ea1e0 {
public:
    unsigned int sub_004ea1e0();

private:
    unsigned char field_00[0x68b];
};

// sub_004ea1f0: Constructor, __thiscall, no
// stack arguments, ECX-in/EAX-out (this homed in ESI, returned in EAX).
//
// Builds the 3x3 identity in three 16-byte-stride float rows at +0x60,
// +0x70, +0x80 (x/y/z stored, w at +0x6c/+0x7c/+0x8c untouched), then
// re-runs the sub-object constructor sub_00463ff0 (EXE 0x00463ff0,
// symbol ??0Object00463ff0@@QAE@XZ) at this+0, and finally sets the
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

// sub_004eb160: __thiscall member with one stack
// argument (RET 4), returns this in EAX.
//
// Copies the 16-byte float record from the argument to *this with one
// MOVUPS load/store pair; `this` is homed in EDX plus a 4-byte stack slot
// (PUSH ECX; MOV EDX,ECX; MOV [ESP],EDX; MOV EAX,[ESP]; MOV EAX,EDX).
// Byte-twin of sub_00480e80 (same body at EXE 0x00480e80) in the SIMD math
// family around 0x004eb160-0x004eb2c0; 28 call sites (e.g. 0x004ec0f1,
// 0x004eb5bd, 0x0052d5b2) pass a pointer-sized source.
//
// MSVC 7.1 intrinsic declarations: __m128 is declared WITHOUT the
// __declspec(align(16)) of the genuine xmmintrin.h typedef. With the
// alignment, MSVC 13.10.3077 wraps any intrinsic body in an EBP frame with
// AND ESP,~0xF realignment; without it the MOVUPS pair lowers inline.
//
// NONMATCHING (score 5, 23 vs 25 bytes): same residual as the byte-twin
// sub_00480e80 (which exhausted ~20 shapes) — the reference stores `this`
// into the homing slot BEFORE reading the argument and reloads it after
// the MOVUPS load, returning from EDX; every /O2 shape hoists the argument
// load above the store. Tried here: volatile destination pointer, volatile
// this pointer, reference binding (score 7, slot dropped), plain
// assignment, and a pointer local with an inlined pointer-to-pointer
// helper (score 7, fully promoted).
typedef struct __declspec(intrin_type) __m128 {
    float m128_f32[4];
} __m128;

extern "C" {
__m128 _mm_loadu_ps(const float *v);
void _mm_storeu_ps(float *v, __m128 a);
}

class Object004eb160 {
public:
    float field_00[4];

    Object004eb160 *sub_004eb160(const Object004eb160 *src);
};

// FUNCTION: RBR 0x004ea140
void Object004ea140::sub_004ea140(unsigned int value)
{
    field_04 = value;
}

// FUNCTION: RBR 0x004ea160
void *Object004ea160::sub_004ea160(unsigned int flags)
{
    sub_004ea0b0();
    if (flags & 1)
        sub_006efdd0(this);
    return this;
}

// FUNCTION: RBR 0x004ea180
Object004ea180::Object004ea180()
{
    vtable = (unsigned int)gVtable0071be94;
}

// FUNCTION: RBR 0x004ea1a0
extern "C" int __stdcall sub_004ea1a0(void *receiver)
{
    (void)receiver;
    return 1;
}

// FUNCTION: RBR 0x004ea1b0
void *Object004ea1b0::sub_004ea1b0(unsigned int flags)
{
    vtable = (unsigned int)gVtable0071be94;
    if (flags & 1)
        sub_006efdd0(this);
    return this;
}

// FUNCTION: RBR 0x004ea1e0
unsigned int Object004ea1e0::sub_004ea1e0()
{
    return (field_00[0x68a] >> 2) & 1;
}

// NONMATCHING
// FUNCTION: RBR 0x004ea1f0
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

// NONMATCHING
// FUNCTION: RBR 0x004eb160
Object004eb160 *Object004eb160::sub_004eb160(const Object004eb160 *src)
{
    float *volatile destination = field_00;
    _mm_storeu_ps(destination, _mm_loadu_ps(src->field_00));
    return this;
}
