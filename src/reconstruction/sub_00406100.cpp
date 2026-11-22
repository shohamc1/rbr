// sub_00406100: EXE 0x00406100, 61 bytes. MSVC 7.1 (/O2 /Zl /arch:SSE).
//
// thiscall twin of sub_00406040/00406080 with a reciprocal: scales the
// receiver's three-float vector by (1.0f / scale) into a caller-provided
// destination. Registers and bytes match the reference: z=XMM1, y=XMM2,
// x=XMM3 (loaded in reverse), inv=XMM0, multiplies in z,y,x order, stores
// forward. The 1.0f literal is the linker-folded shared constant at
// 0x00776a3c (also holds 2^32/1.0/0.5/255.0), addressed by its pinned
// address so the object defines no local data.
//
// NONMATCHING (4 reordered instructions): MSVC 13.10.3077 always schedules
// the reciprocal first (MOVSS XMM0,[0x776a3c]; three loads; DIVSS;
// MOV EAX,[esp+4]), while the reference reads the destination pointer
// between the loads and the reciprocal (loads; MOV EAX; MOVSS; DIVSS).
// Tried without effect (identical 61 bytes in every case): inv-first and
// temps-then-*= statement orders, single-expression forms, an extern/
// extern-volatile global for 1.0f, a destination local, returning the
// destination, #pragma optimize("s"/"g"/"t"), /Og-, /O1, /Ox, /Op, /G6,
// /G7, /Ot. The reference order looks like statement-linear emission that
// no tested shape reproduces.

struct Object00406100 {
    float field_0[3];
    void sub_00406100(float* destination, float scale);
};

void Object00406100::sub_00406100(float* destination, float scale) {
    float z = field_0[2];
    float y = field_0[1];
    float x = field_0[0];
    float inv = *(const float*)0x00776a3c / scale;
    z *= inv;
    y *= inv;
    x *= inv;
    destination[0] = x;
    destination[1] = y;
    destination[2] = z;
}
