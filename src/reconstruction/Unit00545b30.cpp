// sub_00545b30: __thiscall member, no stack
// arguments (base destructor of the class whose vtable slot 0 is
// sub_00545b00). Seeds field_00 with vtable 0x00726524 (slots: sub_00545b00,
// 0x00572440, 0x00572480, 0x005722f0, ..., sub_00545ae0, sub_00545af0) and
// tail-jumps into sub_00572230, which reads ECX as this (vtable re-seed
// 0x00728678, member array walk, cdecl releases of +0xc and +0x10 through
// sub_0056f160). The cdecl alias call reproduces the JMP while leaving ECX
// = this untouched. Compiled with MSVC 7.1 (/O2 /Zl).

extern "C" const void* const gVtable00726524[]; // EXE .rdata 0x00726524

class Object00545b30 {
public:
    unsigned int vtable;
    void sub_00545b30();
};

extern "C" void __cdecl sub_00572230();

// sub_00545b50: __cdecl, no arguments, bare RET.
// Clears the dword flag at EXE 0x0163edbc (sub_005475f0 sets it to 1).
// Registered as a callback from sub_00547d60 at 0x00548250. Compiled with
// MSVC 7.1 (/O2 /Zl).

extern "C" unsigned int gFlag0163edbc; // EXE .data 0x0163edbc

// sub_00545c50: __cdecl with one dword argument
// (registered as a callback from sub_00547d60 at 0x00547d44). The receiver
// object at EXE 0x0163f0a8 is loaded into ECX before the branch: when the
// argument is zero, virtual slot 1 of that object receives (0x6d, 0) via
// CALL [EAX+4]; otherwise the function tail-jumps into sub_0056f440
// (stub in config/symbols.csv; ), which reads the same ECX as this (PUSH ESI;
// MOV ESI,ECX prologue, fields +8/+0xc/+0x14/+0x18/+0x2c, bare RET) — a
// thiscall callee, so the call goes through a same-shape member to keep
// ECX live across both paths (queue: symbol ?sub_0056f440@Object0056f440@@QAEXXZ).
// Compiled with MSVC 7.1 (/O2 /Zl).

class Object0163f0a8 {
public:
    virtual void sub_007264e4_slot0();
    virtual void slot_04(unsigned int a, unsigned int b);
};

class Object0056f440 {
public:
    void sub_0056f440();
};

extern "C" Object0163f0a8* gObject0163f0a8; // EXE .data 0x0163f0a8

// FUNCTION: RBR 0x00545b30
void Object00545b30::sub_00545b30() {
    vtable = (unsigned int)gVtable00726524;
    sub_00572230();
}

// FUNCTION: RBR 0x00545b50
extern "C" void __cdecl sub_00545b50() {
    gFlag0163edbc = 0;
}

// FUNCTION: RBR 0x00545c50
extern "C" void __cdecl sub_00545c50(unsigned int argument) {
    Object0163f0a8 *receiver = gObject0163f0a8;
    if (argument == 0) {
        receiver->slot_04(0x6d, 0);
    } else {
        ((Object0056f440 *)(void *)receiver)->sub_0056f440();
    }
}
