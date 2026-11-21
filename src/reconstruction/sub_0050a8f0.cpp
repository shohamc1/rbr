// sub_0050a8f0: EXE 0x0050a8f0, 27 bytes. MSVC 7.1 (/O2 /Zl).
//
// Scalar deleting destructor, vtable slot 0 of the vtable at 0x0071e03c (same
// family as sub_00402980). Calls the real destructor sub_00503210 (547-byte
// body, thiscall by register; its manifest entry is the cdecl symbol
// _sub_00503210, so it is called with no arguments while ECX still holds
// this), then frees the object through the CRT operator-delete thunk
// sub_006efdd0 (jmp [__imp_??3@YAXPAX@Z]) when bit 0 of the flags argument is
// set. thiscall, one stack argument, returns this in EAX.

extern "C" void __cdecl sub_00503210();             // 0x00503210, real destructor (ECX = this)
extern "C" void __cdecl sub_006efdd0(void* memory); // 0x006efdd0, operator delete thunk

struct Object0050a8f0 {
    void* sub_0050a8f0(unsigned int flags);
};

void* Object0050a8f0::sub_0050a8f0(unsigned int flags) {
    sub_00503210();
    if (flags & 1) {
        sub_006efdd0(this);
    }
    return this;
}
