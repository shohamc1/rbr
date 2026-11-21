// sub_00406080: EXE 0x00406080, 53 bytes. MSVC 7.1 (/O2 /Zl /arch:SSE).
//
// thiscall twin of sub_00406040: scales the receiver's three-float vector by
// a scalar into a caller-provided destination. Same SSE shape (loads z, y, x,
// multiplies, stores forward), with the source addressed from ECX. Arguments:
// float pointer destination at [esp+4], float scale at [esp+8]; RET 8.

struct Object00406080 {
    float field_0[3];
    void sub_00406080(float* destination, float scale);
};

void Object00406080::sub_00406080(float* destination, float scale) {
    float z = field_0[2] * scale;
    float y = field_0[1] * scale;
    float x = field_0[0] * scale;
    destination[0] = x;
    destination[1] = y;
    destination[2] = z;
}
