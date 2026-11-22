// sub_00463ff0: EXE 0x00463ff0, 17 bytes. Default constructor, __thiscall,
// no parameters, returns this in EAX.
//
// Zeroes the 16-byte float record with XORPS/MOVUPS (SSE intrinsics on a
// float[4] record; the movups/movss mix in the sibling zero ctor at
// 0x00480f60 confirms intrinsic-shaped zeroing in this family). Called from
// 243 sites (e.g. 0x00486603/0x00486619 with ECX = sub-objects at +0x4e0);
// callers ignore EAX.
//
// MSVC 7.1 intrinsic declarations: the genuine __declspec(intrin_type)
// __m128 typedef from xmmintrin.h is reproduced locally because the Wine
// image has no headers.
//
// NONMATCHING: the arithmetic and all SSE stores are exact, but the
// reference keeps `this` in a 4-byte stack slot (PUSH ECX; MOV [ESP],ECX;
// MOV EAX,[ESP] ... MOV EAX,ECX; POP ECX; RET). MSVC 13.10.3077 at /O2
// (with or without /arch:SSE, /O1, optimize("s"/"g"), /Ob1, /G5/G7) never
// homes `this` for this body; 20+ source shapes tried (members, helpers by
// pointer/reference/array-reference, loops, bases, __m128 temps). The same
// homing appears across the whole SIMD math family (30 EXE sites, e.g.
// 0x004640b0, 0x004f3500-0x004f3950, 0x004dc660, 0x0043b0e0), suggesting a
// build-mode difference (for example /GL link-time code generation) for
// that library's objects.

typedef struct __declspec(intrin_type) __declspec(align(16)) __m128 {
    float m128_f32[4];
} __m128;

extern "C" {
__m128 _mm_setzero_ps(void);
void _mm_storeu_ps(float* v, __m128 a);
}

class Object00463ff0 {
public:
    float field_00[4];

    Object00463ff0();
};

Object00463ff0::Object00463ff0() {
    _mm_storeu_ps(field_00, _mm_setzero_ps());
}
