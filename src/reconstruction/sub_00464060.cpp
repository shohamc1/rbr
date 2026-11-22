// sub_00464060: EXE 0x00464060, 29 bytes. Default constructor, __thiscall,
// no parameters, returns this in EAX.
//
// Zeroes the 64-byte float record (a 4x4-float matrix) with XORPS plus four
// MOVUPS stores. Callers (e.g. sub_00464330 at 0x00464333) then write 1.0f
// at +0x00 and +0x3c. Compare the 16-byte sibling sub_00463ff0.
//
// MSVC 7.1 intrinsic declarations: the genuine __declspec(intrin_type)
// __m128 typedef from xmmintrin.h is reproduced locally because the Wine
// image has no headers.
//
// NONMATCHING: identical to the reference except the `this` homing slot
// (PUSH ECX; MOV [ESP],ECX; MOV EAX,[ESP] ... MOV EAX,ECX; POP ECX); see
// the sub_00463ff0 note. The dance-free body (21 bytes) is what MSVC
// 13.10.3077 emits at /O2 for every source shape tried.

typedef struct __declspec(intrin_type) __declspec(align(16)) __m128 {
    float m128_f32[4];
} __m128;

extern "C" {
__m128 _mm_setzero_ps(void);
void _mm_storeu_ps(float* v, __m128 a);
}

class Object00464060 {
public:
    float field_00[16];

    Object00464060();
};

Object00464060::Object00464060() {
    __m128 zero = _mm_setzero_ps();
    _mm_storeu_ps(&field_00[0], zero);
    _mm_storeu_ps(&field_00[4], zero);
    _mm_storeu_ps(&field_00[8], zero);
    _mm_storeu_ps(&field_00[12], zero);
}
