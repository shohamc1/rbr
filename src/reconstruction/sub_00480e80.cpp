// sub_00480e80: EXE 0x00480e80, 25 bytes. __thiscall member with one stack
// argument (RET 4), returns this in EAX.
//
// Copies the 16-byte float record from the argument to *this with one
// MOVUPS load/store pair; `this` is homed in EDX plus a 4-byte stack slot
// (PUSH ECX; MOV EDX,ECX; MOV [ESP],EDX; MOV EAX,[ESP]; MOV EAX,EDX).
// Member of the SIMD math family around 0x00480e80-0x00480f60 together with
// sub_00480ec0 and sub_00480ef0 (see sub_00463ff0's note for the family's
// homing signature); no incoming references are known, the layout is body
// evidence.
//
// MSVC 7.1 intrinsic declarations: __m128 is declared WITHOUT the
// __declspec(align(16)) of the genuine xmmintrin.h typedef. With the
// alignment, MSVC 13.10.3077 wraps any intrinsic body in an EBP frame with
// AND ESP,~0xF realignment; without it the MOVUPS pair lowers inline.
//
// NONMATCHING (4 instructions): the arithmetic is exact, but the reference
// stores `this` to the homing slot BEFORE reading the argument and returns
// from EDX (MOV EDX,ECX; MOV [ESP],EDX; MOV ECX,[ESP+8]; MOVUPS XMM0,[ECX];
// MOV EAX,[ESP]; MOVUPS [EAX],XMM0; MOV EAX,EDX), while this draft loads the
// argument first (MOV EAX,ECX; MOV ECX,[ESP+8]; MOVUPS XMM0,[ECX];
// MOV [ESP],EAX; MOV EDX,[ESP]; MOVUPS [EDX],XMM0) with `this` homed in EAX.
// Pointer locals, reference bindings, __fastcall/cdecl helper inlining,
// optimize("s"/"g"), /O1, and /arch:SSE all leave the load hoisted above the
// volatile store; 12 shapes recorded. Same family residual as sub_00463ff0
// and sub_00464080.

typedef struct __declspec(intrin_type) __m128 {
    float m128_f32[4];
} __m128;

extern "C" {
__m128 _mm_loadu_ps(const float* v);
void _mm_storeu_ps(float* v, __m128 a);
}

class Object00480e80 {
public:
    float field_00[4];

    Object00480e80* sub_00480e80(const Object00480e80* src);
};

Object00480e80* Object00480e80::sub_00480e80(const Object00480e80* src) {
    float* volatile dst = field_00;
    _mm_storeu_ps(dst, _mm_loadu_ps(src->field_00));
    return this;
}
