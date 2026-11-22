// sub_00471790: EXE 0x00471790, 52 bytes. MSVC 7.1 (/O2 /Zl).
//
// SSE length of the receiver's 3D vector: loads four components with
// MOVUPS, squares them packed, folds the first three squares with two
// in-place SHUFPS 0x99 rotations and ADDSS, takes SQRTSS, and returns the
// float through x87 (MOVSS to a stack slot, FLD, ST(0) return). thiscall
// member, bare RET. Callers across the EXE (about 90 sites, e.g. 0x0051c017)
// FCOMP the ST(0) result against a radius. Widely shared math helper; the
// receiver record is a bare three-float vector (12+ bytes) at +0.
//
// MSVC 7.1 intrinsic declarations: __m128 is declared WITHOUT the
// __declspec(align(16)) of the genuine xmmintrin.h typedef; with the
// alignment MSVC 13.10.3077 wraps the body in an EBP frame with
// AND ESP,~0xF realignment.
//
// NONMATCHING (score 19, 81 vs 52 bytes): the load, MULPS, the copy of the
// squares into the accumulator register, the first in-place SHUFPS 0x99 and
// ADDSS, and the SQRTSS/MOVSS/FLD return all reproduce (the copy compiles
// as MOVAPS; the reference uses the MOVUPS encoding). The residual is the
// same build-mode difference as the SIMD math family
// (sub_00463ff0/sub_00480e80/sub_004eb160/sub_00480ec0): the reference
// keeps `this` homed in a 4-byte stack slot (SUB ESP,8; MOV [ESP],ECX;
// MOV EAX,[ESP]) and runs the SECOND rotation in place in XMM0 with the
// accumulator staying in XMM1, while MSVC 7.1 /O2 copies the second
// rotation to XMM2, re-homes the accumulator to XMM0, and spills once
// (MOVAPS to stack, which also forces the EBP+AND ESP,~0xF frame).
// Seven shapes tried (reused vs fresh chain and accumulator names, nested
// vs staged shuffles, an unrolled-candidate loop that stays a loop, and
// #pragma optimize("s")); all produce the same round-2 copies. With or
// without /arch:SSE makes no difference (the flag request was withdrawn).
// The this-homing itself is reproduced with a volatile pointer local, as
// in sub_00480ec0.

typedef struct __declspec(intrin_type) __m128 {
    float m128_f32[4];
} __m128;

extern "C" {
__m128 _mm_loadu_ps(const float *v);
__m128 _mm_mul_ps(__m128 a, __m128 b);
__m128 _mm_shuffle_ps(__m128 a, __m128 b, unsigned int imm);
__m128 _mm_add_ss(__m128 a, __m128 b);
__m128 _mm_sqrt_ss(__m128 a);
}

#define VISIT_SHUFFLE(z, y, x, w) (((z) << 6) | ((y) << 4) | ((x) << 2) | (w))

class Object00471790 {
public:
    float field_00[4];   // vector x/y/z (+w padding read by the load)

    float sub_00471790();
};

float Object00471790::sub_00471790() {
    const float *volatile v = field_00;
    __m128 rot = _mm_loadu_ps(v);
    rot = _mm_mul_ps(rot, rot);
    __m128 sum = rot;
    rot = _mm_shuffle_ps(rot, rot, VISIT_SHUFFLE(2, 1, 2, 1));
    __m128 sum2 = _mm_add_ss(sum, rot);
    rot = _mm_shuffle_ps(rot, rot, VISIT_SHUFFLE(2, 1, 2, 1));
    __m128 sum3 = _mm_add_ss(sum2, rot);
    return _mm_sqrt_ss(sum3).m128_f32[0];
}
