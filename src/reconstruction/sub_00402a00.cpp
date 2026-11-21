// sub_00402a00: EXE 0x00402a00, 11 bytes. MSVC 7.1 (/O2 /Zl).
//
// Destructor thunk of the SSO string class whose deleting destructor is
// sub_00402980 (vtable 0x0070c79c; this is the body the deleting destructor
// calls at 0x00402983). Stores the class vtable constant 0x0070c79c into
// this+0 and tail-jumps into the base-class destructor body sub_004029a0
// (EXE 0x004029a0), which stores vtable 0x0070c790, destroys the
// basic_string at +0xc through the MSVCP71 import at IAT 0x0070c2d8 and the
// sub-object through IAT 0x0070c4f8 inside an SEH frame. __thiscall member
// of Object00402980 (shared class shape with sub_00402980.cpp); ECX passes
// through unchanged, EAX/flags are whatever the callee leaves.

class Object004029a0 {
public:
    void sub_004029a0();   // base-class destructor body, EXE 0x004029a0
};

class Object00402980 {
public:
    void* field_00;   // vtable slot; 0x0070c79c for this class
    void sub_00402a00();
};

void Object00402980::sub_00402a00() {
    field_00 = (void*)0x70c79c;
    ((Object004029a0*)this)->sub_004029a0();
}
