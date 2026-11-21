// sub_00402980: EXE 0x00402980, 30 bytes. MSVC 7.1 (/O2 /Zl).
//
// Deleting-destructor body of the class whose vtable is 0x0070c79c (this is
// vtable slot 0; slot 1 is the SSO getter sub_00402970). Calls the destructor
// thunk sub_00402a00 (thiscall: sets the vtable word to 0x0070c79c and jumps
// into the body at 0x004029a0), then frees the object through the CRT thunk
// sub_006efdd0 (jmp [__imp_??3@YAXPAX@Z], operator delete) when bit 0 of the
// flags argument is set. thiscall, one stack argument, returns this in EAX.

extern "C" void __cdecl sub_006efdd0(void* memory);  // 0x006efdd0, operator delete thunk

struct Object00402980 {
    void* sub_00402980(unsigned int flags);
    void sub_00402a00();  // 0x00402a00, destructor thunk (thiscall)
};

void* Object00402980::sub_00402980(unsigned int flags) {
    sub_00402a00();
    if (flags & 1) {
        sub_006efdd0(this);
    }
    return this;
}
