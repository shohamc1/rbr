// sub_00548440: EXE 0x00548440, 13 bytes. __thiscall constructor, no stack
// arguments, ECX-in/EAX-out (MOV EAX,ECX prologue). Zeroes the byte at +0
// and stores -1 in the dword at +0x20. No EXE caller is indexed (likely
// embedded or reached through the sub_00547d60 registration block).
// Compiled with MSVC 7.1 (/O2 /Zl).

class Object00548440 {
public:
    unsigned char field_00;
    unsigned char field_01[0x1f];
    unsigned int field_20;
    Object00548440();
};

typedef char Object00548440_offset20[((unsigned int)&(((Object00548440 *)0)->field_20) == 0x20) ? 1 : -1];

Object00548440::Object00548440() {
    field_00 = 0;
    field_20 = 0xffffffff;
}
