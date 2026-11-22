// sub_00545ad0: EXE 0x00545ad0, 13 bytes. __thiscall setter with one dword
// stack argument (RET 0x4): stores it at +0xd4. sub_00545af0 loads the same
// field and tail-jumps through it, so the value is a callback; both act on
// the class whose vtable sub_00545b30 seeds at 0x00726524 (there sub_00545af0
// sits in slot 0x34). Compiled with MSVC 7.1 (/O2 /Zl).

class Object00545ad0 {
public:
    unsigned char unknown_00[0xd4];
    unsigned int field_d4;
    void sub_00545ad0(unsigned int callback);
};

typedef char Object00545ad0_offset[((unsigned int)&(((Object00545ad0 *)0)->field_d4) == 0xd4) ? 1 : -1];

void Object00545ad0::sub_00545ad0(unsigned int callback) {
    field_d4 = callback;
}
