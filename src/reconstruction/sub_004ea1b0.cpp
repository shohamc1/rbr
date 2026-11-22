// sub_004ea1b0: EXE 0x004ea1b0, 31 bytes. Scalar deleting destructor of the
// class whose vtable is 0x0071be94: __thiscall, one stack argument (the
// destroy/delete flags, RET 4), returns this in EAX.
// Tests flag bit 0 (delete) before homing this, re-seeds the vtable, and on
// the delete path frees this through the cdecl operator-delete thunk
// sub_006efdd0 (EXE 0x006efdd0) with add-esp cleanup. Vtable slot 0 of
// 0x0071be94; the matching default constructor is sub_004ea180.
extern "C" void __cdecl sub_006efdd0(void *memory);

class Object004ea1b0 {
public:
    void *sub_004ea1b0(unsigned int flags);

private:
    unsigned int vtable;
};

void *Object004ea1b0::sub_004ea1b0(unsigned int flags)
{
    vtable = 0x71be94;
    if (flags & 1)
        sub_006efdd0(this);
    return this;
}
