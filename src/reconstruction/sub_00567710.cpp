// sub_00567710: EXE 0x00567710, 30 bytes (RET 4). Scalar deleting destructor,
// vtable slot 0 of the vtable at 0x00727e3c — the class whose constructor is
// sub_005676f0 and finalizer body sub_00567670. Calls the finalizer
// sub_00567670 (thiscall through its manifest class Object00567670, ECX
// unchanged from entry), then frees the object through the CRT operator-
// delete thunk sub_006efdd0 (jmp [__imp_??3@YAXPAX@Z]) when bit 0 of the
// flags argument is set; returns this in EAX. thiscall, one stack argument.
// Same idiom as the matched sub_0050a8f0. Compiled with MSVC 7.1 (/O2 /Zl).

class Object00567670 {
public:
    void sub_00567670();   // finalizer body, EXE 0x00567670
};

extern "C" void __cdecl sub_006efdd0(void *memory);  // 0x006efdd0, operator delete thunk

class Object00567710 {
public:
    void *sub_00567710(unsigned int flags);
};

void *Object00567710::sub_00567710(unsigned int flags) {
    ((Object00567670 *)this)->sub_00567670();
    if (flags & 1) {
        sub_006efdd0(this);
    }
    return this;
}
