// Build flags: /arch:SSE

// sub_004eb180: __thiscall member with two stack
// arguments (RET 8): computes out = normalize(cross(this, rhs)), returns
// the out pointer in EAX. Needs /arch:SSE (packed arithmetic).
//
// SSE lane schedule: both vectors load with MOVUPS; SHUFPS 0xc9 builds the
// cross-product operand pairs, MULPS/SUBPS/MULPS give the squared lanes,
// SHUFPS 0x99 twice rotates the dot accumulator, RSQRTSS + SHUFPS 0
// broadcasts the inverse length, MULPS scales, MOVUPS stores to out.
// `this` is homed in a 4-byte stack slot (PUSH ECX; MOV [ESP],ECX;
// MOV EAX,[ESP]) — the SIMD math family homing (see sub_00463ff0's note);
// family members 0x004ec0e0/0x004ec480/0x004ee3a0/0x004f5fb0/0x005ca9f0
// call this member.
//
// MSVC 7.1 intrinsic declarations: __m128 is declared WITHOUT the
// __declspec(align(16)) of the genuine xmmintrin.h typedef so the body
// lowers inline (here align(16) makes no difference — the canopy below
// forces the frame on its own).
//
// NONMATCHING (score 33, 118 vs 96 bytes): same /GL-family wall as the
// cluster-10 siblings (sub_00480ec0/sub_00480ef0) plus two MSVC 13.10.3077
// lowering fixed points this body exposes:
// 1. _mm_rsqrt_ss always round-trips memory (MOVAPS [ESP]; RSQRTSS;
//    MOVSS [ESP]; MOVAPS reload) and thereby drags in the EBP realignment
//    frame — eight shapes (nested/interleaved copies, per-operand temp
//    order, __m128 with align(16), named-member __m128, #pragma intrinsic,
//    genuine C++-linkage declarations) never produce the reference's
//    register-only RSQRTSS;
// 2. same-operand _mm_shuffle_ps always copies into a fresh register
//    (MOVAPS dst,src; SHUFPS dst,src) where the reference shuffles
//    XMM0/XMM1 in place;
// plus the family `this` homing (reference: PUSH ECX slot store, arg into
// ECX, slot reload into EAX; compiled: this stays in ECX, arg into EAX).
typedef struct __declspec(intrin_type) __m128 {
    float m128_f32[4];
} __m128;

extern "C" {
__m128 _mm_loadu_ps(const float *v);
void _mm_storeu_ps(float *v, __m128 a);
__m128 _mm_shuffle_ps(__m128 a, __m128 b, int imm);
__m128 _mm_mul_ps(__m128 a, __m128 b);
__m128 _mm_sub_ps(__m128 a, __m128 b);
__m128 _mm_add_ss(__m128 a, __m128 b);
__m128 _mm_mul_ss(__m128 a, __m128 b);
__m128 _mm_rsqrt_ss(__m128 a);
}

class Object004eb180 {
public:
    float field_00[4];

    Object004eb180 *sub_004eb180(const Object004eb180 *rhs, Object004eb180 *out);
};

// NONMATCHING
// FUNCTION: RBR 0x004eb180
Object004eb180 *Object004eb180::sub_004eb180(const Object004eb180 *rhs, Object004eb180 *out)
{
    __m128 b = _mm_loadu_ps(rhs->field_00);
    __m128 a = _mm_loadu_ps(field_00);
    __m128 aCopy = a;
    __m128 bCopy = b;
    a = _mm_shuffle_ps(a, a, 0xc9);
    b = _mm_shuffle_ps(b, b, 0xc9);
    __m128 cross = _mm_sub_ps(_mm_mul_ps(aCopy, b), _mm_mul_ps(bCopy, a));
    cross = _mm_shuffle_ps(cross, cross, 0xc9);
    __m128 square = _mm_mul_ps(cross, cross);
    __m128 sum = square;
    square = _mm_shuffle_ps(square, square, 0x99);
    sum = _mm_add_ss(sum, square);
    square = _mm_shuffle_ps(square, square, 0x99);
    sum = _mm_add_ss(sum, square);
    __m128 inverse = _mm_rsqrt_ss(sum);
    inverse = _mm_shuffle_ps(inverse, inverse, 0);
    cross = _mm_mul_ps(cross, inverse);
    _mm_storeu_ps(out->field_00, cross);
    return out;
}
