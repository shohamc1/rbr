// sub_00547690: EXE 0x00547690, 94 bytes. __thiscall constructor with three
// 12-byte by-value structs plus one dword (RET 0x28 = 3*0xc + 4), ECX-in/
// EAX-out (MOV EAX,ECX after the first argument load). The LEA ECX/EDX
// ping-pong with one base per triple (+0x08, +0x14, +0x20) is the MSVC
// by-value struct-copy shape, not ten independent dword stores. Copies the
// three triples into +0x08..+0x28, then field_04 = 0, vtable = 0x7265ac
// (overrides of base vtable 0x007264e4 at +0 and +0xc; base constructor
// sub_00545a50 inlined, not called), then the dword into +0x2c, with its
// stack load hoisted above the two immediate stores. Callers: sub_00547d60
// at 0x00548121, sub_005529a0 at 0x00552c42, sub_0055a670, sub_0055aa30.
// Compiled with MSVC 7.1 (/O2 /Zl).

extern "C" const void* const gVtable007265ac[]; // EXE .rdata 0x007265ac

class Object00547690 {
public:
    struct Triple {
        unsigned int x, y, z;
    };
    unsigned int vtable;
    unsigned int field_04;
    Triple field_08;
    Triple field_14;
    Triple field_20;
    unsigned int field_2c;
    Object00547690(Triple first, Triple second, Triple third, unsigned int last);
};

typedef char Object00547690_offset08[((unsigned int)&(((Object00547690 *)0)->field_08) == 0x08) ? 1 : -1];
typedef char Object00547690_offset14[((unsigned int)&(((Object00547690 *)0)->field_14) == 0x14) ? 1 : -1];
typedef char Object00547690_offset20[((unsigned int)&(((Object00547690 *)0)->field_20) == 0x20) ? 1 : -1];
typedef char Object00547690_offset2c[((unsigned int)&(((Object00547690 *)0)->field_2c) == 0x2c) ? 1 : -1];

Object00547690::Object00547690(Triple first, Triple second, Triple third,
                               unsigned int last) {
    field_08 = first;
    field_14 = second;
    field_20 = third;
    field_04 = 0;
    vtable = (unsigned int)gVtable007265ac;
    field_2c = last;
}
