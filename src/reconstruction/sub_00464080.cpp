// Build flags: /arch:SSE

// sub_00464080: Copy constructor, __thiscall with
// one stack argument (the source), returns this in EAX.
//
// Copies the 0x30-byte float record with three MOVUPS load/store pairs:
// loads into XMM0/XMM1/XMM2 first, then the three stores through EAX.
// `this` is kept in EDX and a 4-byte stack slot (PUSH ECX; MOV [ESP],EDX;
// MOV EAX,[ESP]) and returned from EDX. Callers (e.g. 0x0046469b:
// PUSH result; LEA ECX,[ESP+0x40]; CALL) copy-construct from a
// pointer-sized source; thunks 0x005e0fb0/0x005e0fc0 tail-jump into the
// identical sibling 0x0043b110 with ECX+0/+0x10.
//
// MSVC 7.1 intrinsic declarations: the genuine __declspec(intrin_type)
// __m128 typedef from xmmintrin.h is reproduced locally because the Wine
// image has no headers.
//
// NONMATCHING (12 instructions): MSVC 13.10.3077 /O2 emits the right
// movups pairs but (a) wraps the body in a realigned EBP frame
// (PUSH EBP; MOV EBP,ESP ... AND ESP,~0xf ... MOV ESP,EBP; POP EBP),
// (b) interleaves each load before its store instead of loading
// XMM0/XMM1/XMM2 first, and (c) never homes `this` in the 4-byte slot.
// Cast-based `*(__m128*)` copies drop the EBP frame but emit MOVAPS;
// typed __m128 members also give MOVAPS. The `this`-homing slot appears
// across the whole SIMD math family (see the sub_00463ff0 note).

typedef struct __declspec(intrin_type) __declspec(align(16)) __m128 {
    float m128_f32[4];
} __m128;

extern "C" {
void _mm_storeu_ps(float* v, __m128 a);
__m128 _mm_loadu_ps(const float* v);
}

class Object00464080 {
public:
    float field_00[12];

    Object00464080(const Object00464080& other);
};

// NONMATCHING
// FUNCTION: RBR 0x00464080
Object00464080::Object00464080(const Object00464080& other) {
    _mm_storeu_ps(&field_00[0], _mm_loadu_ps(&other.field_00[0]));
    _mm_storeu_ps(&field_00[4], _mm_loadu_ps(&other.field_00[4]));
    _mm_storeu_ps(&field_00[8], _mm_loadu_ps(&other.field_00[8]));
}
