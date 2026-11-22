// sub_004ea160: EXE 0x004ea160, 30 bytes. Scalar deleting destructor of the
// class whose vtable is 0x0071be8c: __thiscall, one stack argument (the
// destroy/delete flags, RET 4), returns this in EAX.
// Homes this in ESI, calls the destructor body sub_004ea0b0 (EXE 0x004ea0b0,
// manifest source:null; seeds vtable 0x71be8c and frees the 256 pointer
// fields at this+8..this+0x404 through sub_006efdd0) without resetting ECX
// to this — the call relies on the incoming ECX home, so the declaration
// stays cdecl. On flag bit 0 frees this through the cdecl operator-delete
// thunk sub_006efdd0 (EXE 0x006efdd0) with add-esp cleanup.
// Vtable slot 0 of 0x0071be8c (referenced by data at 0x0071be8c).
extern "C" void __cdecl sub_004ea0b0();
extern "C" void __cdecl sub_006efdd0(void *memory);

class Object004ea160 {
public:
    void *sub_004ea160(unsigned int flags);
};

void *Object004ea160::sub_004ea160(unsigned int flags)
{
    sub_004ea0b0();
    if (flags & 1)
        sub_006efdd0(this);
    return this;
}
