// sub_00678e7e: EXE 0x00678e7e, 29 bytes. MSVC 7.1 (/O2 /Zl).
//
// Teardown member (real destructor body) of the class whose scalar deleting
// destructor is sub_0067c65d (vtable 0x0073c904 slot 0; the matched caller
// tail-calls this member). Declared on the caller's class Object0067c65d so
// one symbol resolves the call site.
//
// Frees the pointer at +0xc8 through the CRT operator-delete thunk
// sub_006efdd0 (jmp [__imp_??3@YAXPAX@Z]) with POP ECX cdecl cleanup,
// restores the vtable word at +0 to 0x0073c904, then tail-jumps to the
// base-class teardown member sub_0067884c (thiscall, stores vtable
// 0x0073c900) with this in ECX.

#pragma optimize("s", on)

extern "C" const void* const gVtable0073c904[]; // EXE .rdata 0x0073c904

extern "C" void __cdecl sub_006efdd0(void* memory);  // 0x006efdd0, operator delete thunk

class Object0067c65d {
public:
    void* vtable_00;
    unsigned char unknown_04[0xc4];
    void* field_c8;
    void sub_00678e7e();
    void sub_0067884c();  // 0x0067884c, base-class teardown (thiscall)
};

void Object0067c65d::sub_00678e7e() {
    vtable_00 = (void*)gVtable0073c904;
    sub_006efdd0(field_c8);
    sub_0067884c();
}
