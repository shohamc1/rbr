// sub_0067c65d: EXE 0x0067c65d, 28 bytes. __thiscall with one stack word
// (RET 4): a scalar deleting-destructor shape. The vtable at EXE 0x0073c904
// references this function directly.
//
// It first tail-calls the shared teardown member sub_00678e7e on this (ECX
// is still this, so the call needs no register setup), then, when bit 0 of
// the flags word is set, frees this through sub_006efdd0 (the EXE-internal
// JMP thunk at 0x006efdd0 on the imported operator delete(void*)
// ??3@YAXPAX@Z). Returns this.
//
// The operator-delete argument is cleaned with POP ECX rather than
// ADD ESP,4: the file-local size pragma selects the pop form.

#pragma optimize("s", on)

extern "C" void __cdecl sub_006efdd0(void *memory);

class Object0067c65d {
public:
    void sub_00678e7e();
    void *sub_0067c65d(unsigned int flags);
};

void *Object0067c65d::sub_0067c65d(unsigned int flags) {
    sub_00678e7e();
    if (flags & 1)
        sub_006efdd0(this);
    return this;
}
