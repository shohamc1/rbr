// sub_00480ec0: EXE 0x00480ec0, 46 bytes. __thiscall member with two stack
// arguments (RET 8), returns this in EAX.
//
// Scaled add on the 16-byte float record: this += v * t as one packed SSE
// lane chain (MOVSS t; SHUFPS broadcast; MOVUPS v; MOVUPS this; MULPS;
// ADDPS; MOVUPS store). `this` stays live in ECX for the return and is also
// homed in a 4-byte stack slot (PUSH ECX; MOV [ESP+4],ECX; MOV EAX,[ESP+4])
// with v homed in EBX across the loads. Member of the SIMD math family
// around 0x00480e80-0x00480f60 (see sub_00463ff0's note); the record layout
// is body evidence.
//
// MSVC 7.1 intrinsic declarations: __m128 is declared WITHOUT the
// __declspec(align(16)) of the genuine xmmintrin.h typedef; with the
// alignment MSVC 13.10.3077 wraps the body in an EBP frame with
// AND ESP,~0xF realignment.
//
// NONMATCHING: the packed arithmetic and broadcast are exact, but this
// draft homes the destination through a volatile pointer local whose store
// is scheduled after the argument load, and MSVC reloads the slot once per
// use (two reloads) instead of the reference's single EBX/EAX pass with the
// slot store first. Same family residual as sub_00463ff0/sub_00464080.

typedef struct __declspec(intrin_type) __m128 {
    float m128_f32[4];
} __m128;

extern "C" {
__m128 _mm_loadu_ps(const float* v);
void _mm_storeu_ps(float* v, __m128 a);
__m128 _mm_set_ps1(float w);
__m128 _mm_mul_ps(__m128 a, __m128 b);
__m128 _mm_add_ps(__m128 a, __m128 b);
}

class Object00480ec0 {
public:
    float field_00[4];

    Object00480ec0* sub_00480ec0(const Object00480ec0* v, float t);
};

Object00480ec0* Object00480ec0::sub_00480ec0(const Object00480ec0* v, float t) {
    float* volatile dst = field_00;
    __m128 vv = _mm_loadu_ps(v->field_00);
    __m128 tt = _mm_set_ps1(t);
    __m128 cur = _mm_loadu_ps(dst);
    _mm_storeu_ps(dst, _mm_add_ps(_mm_mul_ps(vv, tt), cur));
    return this;
}
