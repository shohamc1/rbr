// Build flags: /arch:SSE

// sub_00406040: MSVC 7.1 (/O2 /Zl /arch:SSE).
//
// Scales a three-float vector by a scalar: destination[i] = source[i] * scale
// for i = 0..2. The reference loads the source components in reverse (z, y,
// x), multiplies each by the scalar, and stores forward. __cdecl: float
// pointer destination, float scale, const float pointer source; bare RET.

// sub_00406080: MSVC 7.1 (/O2 /Zl /arch:SSE).
//
// thiscall twin of sub_00406040: scales the receiver's three-float vector by
// a scalar into a caller-provided destination. Same SSE shape (loads z, y, x,
// multiplies, stores forward), with the source addressed from ECX. Arguments:
// float pointer destination at [esp+4], float scale at [esp+8]; RET 8.

struct Object00406080 {
    float field_0[3];
    void sub_00406080(float* destination, float scale);
};

// FUNCTION: RBR 0x00406040
extern "C" void __cdecl sub_00406040(float* destination, float scale, const float* source) {
    float z = source[2] * scale;
    float y = source[1] * scale;
    float x = source[0] * scale;
    destination[0] = x;
    destination[1] = y;
    destination[2] = z;
}

// FUNCTION: RBR 0x00406080
void Object00406080::sub_00406080(float* destination, float scale) {
    float z = field_0[2] * scale;
    float y = field_0[1] * scale;
    float x = field_0[0] * scale;
    destination[0] = x;
    destination[1] = y;
    destination[2] = z;
}
