// sub_00545b30: EXE 0x00545b30, 11 bytes. __thiscall member, no stack
// arguments (base destructor of the class whose vtable slot 0 is
// sub_00545b00). Seeds field_00 with vtable 0x00726524 (slots: sub_00545b00,
// 0x00572440, 0x00572480, 0x005722f0, ..., sub_00545ae0, sub_00545af0) and
// tail-jumps into sub_00572230, which reads ECX as this (vtable re-seed
// 0x00728678, member array walk, cdecl releases of +0xc and +0x10 through
// sub_0056f160). The cdecl alias call reproduces the JMP while leaving ECX
// = this untouched. Compiled with MSVC 7.1 (/O2 /Zl).

class Object00545b30 {
public:
    unsigned int vtable;
    void sub_00545b30();
};

extern "C" void __cdecl sub_00572230();

void Object00545b30::sub_00545b30() {
    vtable = 0x726524;
    sub_00572230();
}
