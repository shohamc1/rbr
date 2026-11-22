// sub_00567710: (RET 4). Scalar deleting destructor,
// vtable slot 0 of the vtable at 0x00727e3c — the class whose constructor is
// sub_005676f0 and finalizer body sub_00567670. Calls the finalizer
// sub_00567670 (thiscall through its class Object00567670, ECX
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

// sub_00567730: Constructor of the intrusive-list
// wrapper class whose finalizer is sub_00567690 and deleting finalizer
// sub_00567750 (vtable 0x00727e48, slot 0): MOV EAX,ECX prologue with this
// returned in EAX. The wrapper is a polymorphic head at +0 followed by a
// 3-pointer link node at +4 (links at +4/+8, owning-list pointer at +0xc);
// the constructor zeroes the three node fields — note the observed order
// +8, +4, +0xc — then stores the raw vtable address. Byte-identical to
// sub_005676f0 except the vtable constant. The vtable slot is a plain field
// (raw-address store), so no object-local vtable is emitted and the store
// order stays under source control. Compiled with MSVC 7.1 (/O2 /Zl).

extern "C" const void* const gVtable00727e48[]; // EXE .rdata 0x00727e48

class Object00567730 {
public:
    unsigned int field_00;   // vtable slot; 0x00727e48 for this class
    void *field_04;          // node link
    void *field_08;          // node link
    void *field_0c;          // owning list pointer

    Object00567730();
};

// FUNCTION: RBR 0x00567710
void *Object00567710::sub_00567710(unsigned int flags) {
    ((Object00567670 *)this)->sub_00567670();
    if (flags & 1) {
        sub_006efdd0(this);
    }
    return this;
}

// FUNCTION: RBR 0x00567730
Object00567730::Object00567730() {
    field_08 = 0;
    field_04 = 0;
    field_0c = 0;
    field_00 = (unsigned int)gVtable00727e48;
}
