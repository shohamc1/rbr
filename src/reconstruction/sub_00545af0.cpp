// sub_00545af0: EXE 0x00545af0, 13 bytes. __thiscall member, no stack
// arguments (vtable 0x00726524 slot 0x34; sub_00545ad0 stores the field).
// Loads the cdecl callback at +0xd4 and, when nonzero, tail-jumps through
// it: MOV EAX,[ECX+0xd4]; TEST EAX,EAX; JZ ret; JMP EAX. The callback local
// keeps the guard-and-tail-jump shape. Compiled with MSVC 7.1 (/O2 /Zl).

class Object00545af0 {
public:
    unsigned char unknown_00[0xd4];
    void (__cdecl *field_d4)();
    void sub_00545af0();
};

typedef char Object00545af0_offset[((unsigned int)&(((Object00545af0 *)0)->field_d4) == 0xd4) ? 1 : -1];

void Object00545af0::sub_00545af0() {
    void (__cdecl *callback)() = field_d4;
    if (callback)
        callback();
}
