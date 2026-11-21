// sub_005b02a0: EXE 0x005b02a0, 8 bytes. this-adjust thunk: ECX += 0x5c and
// a tail jump to sub_00492520 (0x00492520, 82 bytes, not in the manifest), a
// __thiscall member that returns a pointer (error string 0x713150 /
// 0x7131b0 or the cached entry at this+4) for the global object at
// 0x00872c40. The thunk's address is stored in data at 0x0072d994, so it is
// a dispatch/vtable slot body. The wrapper is void: the reference ends in a
// bare JMP, so the wrapper owns no frame and no cleanup.

struct Object00492520 {
    void *sub_00492520();
};

struct Object005b02a0 {
    unsigned char unknown_00[0x5c];
    Object00492520 field_5c;

    void sub_005b02a0();
};

void Object005b02a0::sub_005b02a0() {
    field_5c.sub_00492520();
}
