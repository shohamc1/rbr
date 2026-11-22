// sub_00552710: EXE 0x00552710, 9 bytes. __thiscall constructor of a 4-byte
// polymorphic object, ECX-in/EAX-out: MOV EAX,ECX first, then the vtable
// pointer for the dispatch table at EXE 0x00726f50 is stored at +0x00. Slot 0
// of that table is the deleting destructor 0x00552720 (calls base constructor
// 0x00552740, which installs the base vtable 0x007219b8, then operator delete
// sub_0056f190 with size 4). The vtable is written as a raw immediate so the
// object stays free of local RTTI data. No direct references found in the EXE.
// Compiled with MSVC 7.1 (/O2 /Zl).

class Object00552710 {
public:
    unsigned int field_00; // vtable pointer: dispatch table at 0x00726f50
    Object00552710();
};

extern "C" const void* const gVtable00726f50[]; // EXE .rdata 0x00726f50

Object00552710::Object00552710() { field_00 = (unsigned int)gVtable00726f50; }
