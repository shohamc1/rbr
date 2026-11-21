// sub_004eb160: EXE 0x004eb160, 25 bytes. __thiscall member with one stack
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

Object004eb160 *Object004eb160::sub_004eb160(const Object004eb160 *src)
{
    float *volatile destination = field_00;
    _mm_storeu_ps(destination, _mm_loadu_ps(src->field_00));
    return this;
}
