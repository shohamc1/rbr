// sub_0061ea99: EXE 0x0061ea99, 28 bytes. MSVC 7.1 (/O2 /Zl) with the
// file-local size pragma (POP ECX cleanup).
//
// Scalar deleting destructor, slot 0 of the vtable at EXE 0x00734090 (the
// store target of the base teardown sub_0061d989). Calls the base teardown
// member sub_0061d989 on this while ECX still holds this, then frees this
// through sub_006efdd0 when bit 0 of the flags word is set, and returns
// this. __thiscall with one stack word (RET 4).

#pragma optimize("s", on)

extern "C" void __cdecl sub_006efdd0(void* memory);

class Object0061d989 {
public:
    void sub_0061d989();
};

class Object0061ea99 {
public:
    void* sub_0061ea99(unsigned int flags);
};

void* Object0061ea99::sub_0061ea99(unsigned int flags) {
    ((Object0061d989*)this)->sub_0061d989();
    if (flags & 1)
        sub_006efdd0(this);
    return this;
}
