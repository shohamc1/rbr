// Build flags: /arch:SSE

// sub_00480ef0: __thiscall member with two stack
// arguments (RET 8), returns this in EAX.
//
// Packed lerp on the 16-byte float record: this = this + (v - this) * t as
// one SSE lane chain (MOVSS t; SHUFPS broadcast; MOVUPS this into XMM1;
// MOVUPS v into XMM2; SUBPS XMM2,XMM1; MULPS; ADDPS XMM2,XMM1; MOVUPS
// store). `this` is homed in EDX plus a 4-byte stack slot (PUSH ECX;
// MOV EDX,ECX; MOV [ESP],EDX; MOV EAX,[ESP]) and returned from EDX with the
// argument reusing ECX. Callers 0x004852c0 and 0x004efa20 pass v at [ESP+4]
// and t at [ESP+8]. Member of the SIMD math family around
// 0x00480e80-0x00480f60 (see sub_00463ff0's note).
//
// MSVC 7.1 intrinsic declarations: __m128 is declared WITHOUT the
// __declspec(align(16)) of the genuine xmmintrin.h typedef; with the
// alignment MSVC 13.10.3077 wraps the body in an EBP frame with
// AND ESP,~0xF realignment.
//
// NONMATCHING: the packed arithmetic and broadcast are exact, but this
// draft homes the destination through a volatile pointer local whose store
// is scheduled after the argument load, and MSVC reloads the slot once per
// use (two reloads) instead of the reference's single reload with `this`
// kept in EDX across. Same family residual as sub_00463ff0/sub_00464080.

typedef struct __declspec(intrin_type) __m128 {
    float m128_f32[4];
} __m128;

extern "C" {
__m128 _mm_loadu_ps(const float* v);
void _mm_storeu_ps(float* v, __m128 a);
__m128 _mm_set_ps1(float w);
__m128 _mm_sub_ps(__m128 a, __m128 b);
__m128 _mm_mul_ps(__m128 a, __m128 b);
__m128 _mm_add_ps(__m128 a, __m128 b);
}

class Object00480ef0 {
public:
    float field_00[4];

    Object00480ef0* sub_00480ef0(const Object00480ef0* v, float t);
};

// NONMATCHING
// FUNCTION: RBR 0x00480ef0
Object00480ef0* Object00480ef0::sub_00480ef0(const Object00480ef0* v, float t) {
    float* volatile dst = field_00;
    __m128 tt = _mm_set_ps1(t);
    __m128 vv = _mm_loadu_ps(v->field_00);
    __m128 cur = _mm_loadu_ps(dst);
    _mm_storeu_ps(dst, _mm_add_ps(_mm_mul_ps(_mm_sub_ps(vv, cur), tt), cur));
    return this;
}
