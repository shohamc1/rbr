// sub_00406040: EXE 0x00406040, 55 bytes. MSVC 7.1 (/O2 /Zl /arch:SSE).
//
// Scales a three-float vector by a scalar: destination[i] = source[i] * scale
// for i = 0..2. The reference loads the source components in reverse (z, y,
// x), multiplies each by the scalar, and stores forward. __cdecl: float
// pointer destination, float scale, const float pointer source; bare RET.

extern "C" void __cdecl sub_00406040(float* destination, float scale, const float* source) {
    float z = source[2] * scale;
    float y = source[1] * scale;
    float x = source[0] * scale;
    destination[0] = x;
    destination[1] = y;
    destination[2] = z;
}
