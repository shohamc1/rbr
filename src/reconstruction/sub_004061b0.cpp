// sub_004061b0: EXE 0x004061b0, 68 bytes. MSVC 7.1 (/O2 /Zl /arch:SSE).
//
// thiscall four-float add: destination[i] = other[i] + this->field_0[i] for
// i = 0..3. The reference loads the other vector's components in reverse
// (w, z, y, x into XMM0..XMM3), adds the receiver's components from memory
// (ADDSS [ECX+i*4]), and stores forward. Arguments: float pointer
// destination at [esp+4], const float pointer other at [esp+8]; RET 8.

struct Object004061b0 {
    float field_0[4];
    void sub_004061b0(float* destination, const float* other);
};

void Object004061b0::sub_004061b0(float* destination, const float* other) {
    float w = other[3] + field_0[3];
    float z = other[2] + field_0[2];
    float y = other[1] + field_0[1];
    float x = other[0] + field_0[0];
    destination[0] = x;
    destination[1] = y;
    destination[2] = z;
    destination[3] = w;
}
