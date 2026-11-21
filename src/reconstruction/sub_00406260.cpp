// sub_00406260: EXE 0x00406260, 48 bytes. MSVC 7.1 (/O2 /Zl /arch:SSE).
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

Object00406260::Object00406260(float x, float y, float z, float w) {
    field_0[0] = x;
    field_0[1] = y;
    field_0[2] = z;
    field_0[3] = w;
}
