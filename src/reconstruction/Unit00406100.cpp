// Build flags: /arch:SSE

// sub_00406100: MSVC 7.1 (/O2 /Zl /arch:SSE).
//
// thiscall twin of sub_00406040/00406080 with a reciprocal: scales the
// receiver's three-float vector by (1.0f / scale) into a caller-provided
// destination. Registers and bytes match the reference: z=XMM1, y=XMM2,
// x=XMM3 (loaded in reverse), inv=XMM0, multiplies in z,y,x order, stores
// forward. The 1.0f literal resolves to the shared pool float at
// 0x00776a3c. Reading it through a pointer instead made MSVC hoist the
// reciprocal above the loads.

struct Object00406100 {
    float field_0[3];
    void sub_00406100(float* destination, float scale);
};

// sub_004061b0: MSVC 7.1 (/O2 /Zl /arch:SSE).
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

// sub_00406260: MSVC 7.1 (/O2 /Zl /arch:SSE).
//
// Four-float constructor of the vector class in this neighborhood: ECX holds
// the receiver, the four float arguments sit at [esp+4..0x10], each is moved
// through XMM0 into the receiver (stores addressed from EAX after the
// MOV EAX,ECX constructor prologue), and this is returned in EAX. RET 0x10
// pops the four argument slots. Constructor: ECX in, EAX out.

struct Object00406260 {
    float field_0[4];
    Object00406260(float x, float y, float z, float w);
};

// FUNCTION: RBR 0x00406100
void Object00406100::sub_00406100(float* destination, float scale) {
    float z = field_0[2];
    float y = field_0[1];
    float x = field_0[0];
    float inv = 1.0f / scale;
    z *= inv;
    y *= inv;
    x *= inv;
    destination[0] = x;
    destination[1] = y;
    destination[2] = z;
}

// FUNCTION: RBR 0x004061b0
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

// FUNCTION: RBR 0x00406260
Object00406260::Object00406260(float x, float y, float z, float w) {
    field_0[0] = x;
    field_0[1] = y;
    field_0[2] = z;
    field_0[3] = w;
}
