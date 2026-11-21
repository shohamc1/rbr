// sub_0050a910: EXE 0x0050a910, 27 bytes. MSVC 7.1 (/O2 /Zl).
//
// Scalar deleting destructor, vtable slot 0 of the vtable at 0x0071e094 (the
// destructor sub_00503550 stores that vtable address back into the object).
// Calls the real destructor sub_00503550 (61-byte body, thiscall member of the
// same class), then frees the object through the CRT operator-delete thunk
// sub_006efdd0 (jmp [__imp_??3@YAXPAX@Z]) when bit 0 of the flags argument is
// set. Same shape as the matched sub_00402980. thiscall, one stack argument,
// returns this in EAX.

extern "C" void __cdecl sub_006efdd0(void* memory); // 0x006efdd0, operator delete thunk

struct Object0050a910 {
    void* sub_0050a910(unsigned int flags);
    void sub_00503550(); // 0x00503550, real destructor (thiscall)
};

void* Object0050a910::sub_0050a910(unsigned int flags) {
    sub_00503550();
    if (flags & 1) {
        sub_006efdd0(this);
    }
    return this;
}
