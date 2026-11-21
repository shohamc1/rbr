// sub_0061d989: EXE 0x0061d989, 38 bytes. MSVC 7.1 (/O2 /Zl) with the
// file-local size pragma (vtable store scheduled between the first
// argument push and its call; batched ADD ESP,0xc cleanup).
//
// Base teardown member of the hierarchy whose deleting destructor
// sub_0061ea99 sits in slot 0 of the vtable at EXE 0x00734090. Stores the
// base vtable word at +0, then frees the pointers at +0x38, +0x58 and
// +0x5c through the CRT operator-delete thunk sub_006efdd0. __thiscall, no
// stack arguments. Called from sub_0061ea99, the tail of sub_0061e6bf, and
// 37 deleting-destructor wrappers in the 0x0062xxxx range.

#pragma optimize("s", on)

extern "C" void __cdecl sub_006efdd0(void* memory);

class Object0061d989 {
public:
    void* vtable_00;
    unsigned char unknown_04[0x34];
    void* field_38;
    unsigned char unknown_3c[0x1c];
    void* field_58;
    void* field_5c;

    void sub_0061d989();
};

void Object0061d989::sub_0061d989() {
    vtable_00 = (void*)0x00734090;
    sub_006efdd0(field_38);
    sub_006efdd0(field_58);
    sub_006efdd0(field_5c);
}
