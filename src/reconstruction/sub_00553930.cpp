// sub_00553930: EXE 0x00553930, 79 bytes. __thiscall constructor with five
// dword stack arguments (RET 0x14), ECX-in/EAX-out. Stores the arguments at
// +0x00..+0x10 with the same ECX/EDX ping-pong order as sub_004995a0, then
// copies the packed 14-byte record at EXE 0x007270b8 into +0x14..+0x21
// (LEA EDX,[EAX+0x14]; three dwords and one word, both sides packed to
// exactly 14 bytes). The record is the .rdata string "Richard Burns\0"
// (13 characters plus NUL); the 0x44 byte before it ends the float at
// 0x007270b4. The copy source is the extern global gRichardBurnsName
// (config/globals.json); a named global folds to the absolute
// per-field loads the reference uses, unlike a cast constant pointer, and
// #pragma pack(2) keeps the trailing word a word instead of a padded dword.
// The direct caller at EXE 0x005a5f56 constructs a stack temporary at
// [esp+0x84] with arguments (0, 0, [0x0163f2c0]+0x258, +0x25c, +0x140) and
// passes the returned this straight into sub_00553980, confirming the
// ECX-in/EAX-out constructor contract. Compiled with MSVC 7.1 (/O2 /Zl).

#pragma pack(push, 2)
struct Tail00553930 {
    unsigned int f0;
    unsigned int f4;
    unsigned int f8;
    unsigned short fc;
};
#pragma pack(pop)

extern "C" Tail00553930 gRichardBurnsName; // "Richard Burns\0" at EXE 0x007270b8

class Object00553930 {
public:
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
    unsigned int field_10;
    Tail00553930 field_14;
    Object00553930(unsigned int a, unsigned int b, unsigned int c, unsigned int d,
                   unsigned int e);
};

typedef char Object00553930_offset[((unsigned int)&(((Object00553930*)0)->field_14) == 0x14) ? 1 : -1];

Object00553930::Object00553930(unsigned int a, unsigned int b, unsigned int c,
                               unsigned int d, unsigned int e) {
    field_00 = a;
    field_04 = b;
    field_08 = c;
    field_0c = d;
    field_10 = e;
    field_14 = gRichardBurnsName;
}
