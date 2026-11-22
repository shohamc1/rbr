// sub_005475f0: __cdecl with one dword argument
// (registered as a callback from sub_00547d60 at 0x005478b4). When the
// argument is zero: sets the dword flag at EXE 0x0163edbc to 1 (sub_00545b50
// clears it) and calls virtual slot 1 of the object at EXE 0x0163f0a8 with
// (0x5b, 0) through CALL [EAX+4] — same receiver class as sub_00545c50.
// The flag store rides in the first argument's value computation: that is
// the one source position that lands it inside the push run (PUSH 0,
// store, MOV EAX,[ECX], PUSH 0x5b) as the reference schedules it.
// Compiled with MSVC 7.1 (/O2 /Zl).

class Object0163f0a8 {
public:
    virtual void sub_007264e4_slot0();
    virtual void slot_04(unsigned int a, unsigned int b);
};

extern "C" Object0163f0a8* gObject0163f0a8; // EXE .data 0x0163f0a8
extern "C" unsigned int gFlag0163edbc;      // EXE .data 0x0163edbc

// sub_00547690: __thiscall constructor with three
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

// sub_00548440: __thiscall constructor, no stack
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

// FUNCTION: RBR 0x005475f0
extern "C" void __cdecl sub_005475f0(unsigned int argument) {
    if (argument == 0) {
        Object0163f0a8 *receiver = gObject0163f0a8;
        receiver->slot_04(0x5b, (gFlag0163edbc = 1, 0));
    }
}

// FUNCTION: RBR 0x00547690
Object00547690::Object00547690(Triple first, Triple second, Triple third,
                               unsigned int last) {
    field_08 = first;
    field_14 = second;
    field_20 = third;
    field_04 = 0;
    vtable = (unsigned int)gVtable007265ac;
    field_2c = last;
}

// FUNCTION: RBR 0x00548440
Object00548440::Object00548440() {
    field_00 = 0;
    field_20 = 0xffffffff;
}
