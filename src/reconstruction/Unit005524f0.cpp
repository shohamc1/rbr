// sub_005524f0: __cdecl thunk, no arguments,
// registered as an event callback: EXE 0x00552642 pushes this address into
// virtual slot 0x5c of an object with (0xb, esi, 1, callback). Loads the
// object pointer stored at EXE 0x007b17cc into ECX and tail-jumps to the
// __thiscall member sub_0056e4a0 (0x0056e4a0, 72 bytes, no stack arguments,
// bare RET), so MSVC 7.1 /O2 emits the two-instruction thunk below. Compiled
// with /O2 /Zl.

// Callee class declaration; the callee has its own source.
class Object0056e4a0 {
public:
    void sub_0056e4a0();
};

extern "C" Object0056e4a0* gObject007b17cc; // EXE .data 0x007b17cc

// sub_005526d0: __cdecl callback, no arguments,
// registered at EXE 0x00552d5b (pushed with (0, 1, callback) into sub_005726e0).
// Sets the two name-format string fields of the fixed object at EXE 0x007b184c:
// "qr%02d_in.arp" (0x00726f3c) through sub_0056f250 (stores at +0x0c) and
// "qr%02d_out.arp" (0x00726f2c) through sub_0056f260 (stores at +0x10). Both
// callees are __thiscall members with one cdecl stack argument (RET 4). The
// sibling clear member at 0x0056f200 zeroes +0x00..+0x10 with -1 at +0x04.
// Compiled with MSVC 7.1 (/O2 /Zl).

// Callee class declaration; both callees have their own source.
class Object0056f250 {
public:
    void sub_0056f250(const char *name);
    void sub_0056f260(const char *name);
};

extern "C" Object0056f250 gObject007b184c; // EXE .data 0x007b184c

// sub_00552710: __thiscall constructor of a 4-byte
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

// FUNCTION: RBR 0x005524f0
extern "C" void __cdecl sub_005524f0() {
    gObject007b17cc->sub_0056e4a0();
}

// FUNCTION: RBR 0x005526d0
extern "C" void __cdecl sub_005526d0() {
    gObject007b184c.sub_0056f250("qr%02d_in.arp");
    gObject007b184c.sub_0056f260("qr%02d_out.arp");
}

// FUNCTION: RBR 0x00552710
Object00552710::Object00552710() { field_00 = (unsigned int)gVtable00726f50; }
