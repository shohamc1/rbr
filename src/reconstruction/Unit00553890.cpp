// sub_00553890: __thiscall member, no stack
// arguments, bare RET. Increments the dword at +0x4c and returns the new
// value: the EAX roundtrip (load, one-byte INC EAX, store) only appears when
// the result is used, so the reference reads as a pre-increment with its value
// returned. No direct references found in the EXE; the owner class of +0x4c is
// unproved, keep the local class.

class Object00553890 {
public:
    unsigned char unknown_00[0x4c];
    unsigned int field_4c;
    unsigned int sub_00553890();
};

typedef char Object00553890_offset[((unsigned int)&(((Object00553890*)0)->field_4c) == 0x4c) ? 1 : -1];

// sub_005538a0: __thiscall member with one dword
// stack argument (RET 4). Stores the argument to the dword at +0x468. No
// direct references found in the EXE; the owner class is unproved, keep the
// local class. Compiled with MSVC 7.1 (/O2 /Zl).

class Object005538a0 {
public:
    unsigned char unknown_00[0x468];
    unsigned int field_468;
    void sub_005538a0(unsigned int value);
};

typedef char Object005538a0_offset[((unsigned int)&(((Object005538a0*)0)->field_468) == 0x468) ? 1 : -1];

// sub_005538c0: __thiscall member with one dword
// stack argument (RET 4) that the body never reads: a notification-style
// callback that just sets the dword at +0x38 to 1. No direct references found
// in the EXE; the owner class is unproved, keep the local class. Compiled with
// MSVC 7.1 (/O2 /Zl).

class Object005538c0 {
public:
    unsigned char unknown_00[0x38];
    unsigned int field_38;
    void sub_005538c0(unsigned int unused);
};

typedef char Object005538c0_offset[((unsigned int)&(((Object005538c0*)0)->field_38) == 0x38) ? 1 : -1];

// sub_005538f0: __cdecl, one dword argument, plain
// RET. Predicate on a state code: true when the argument is 0 or 1 (unsigned
// compare, JBE covers both contiguous cases) or exactly 10 (0xa). No direct
// references found in the EXE; dispatch is unexplained (registration or table
// evidence pending). Compiled with MSVC 7.1 (/O2 /Zl); the sparse switch keeps
// the JBE/JNE chain with the case block inline and the default out-of-line.

// sub_00553910: __thiscall constructor with two
// dword stack arguments (RET 8), ECX-in/EAX-out: EAX is loaded from ECX before
// the argument registers clobber it, matching the MSVC 7.1 constructor ABI.
// Stores arg1 at +0x00, arg2 at +0x04, zero at +0x08. Same ECX/EDX ping-pong
// argument order as the matched sub_004995a0 constructor. No direct references
// found in the EXE. Compiled with MSVC 7.1 (/O2 /Zl).

class Object00553910 {
public:
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    Object00553910(unsigned int a, unsigned int b);
};

// sub_00553930: __thiscall constructor with five
// dword stack arguments (RET 0x14), ECX-in/EAX-out. Stores the arguments at
// +0x00..+0x10 with the same ECX/EDX ping-pong order as sub_004995a0, then
// copies the packed 14-byte record at EXE 0x007270b8 into +0x14..+0x21
// (LEA EDX,[EAX+0x14]; three dwords and one word, both sides packed to
// exactly 14 bytes). The record is the .rdata string "Richard Burns\0"
// (13 characters plus NUL); the 0x44 byte before it ends the float at
// 0x007270b4. The copy source is the extern global gRichardBurnsName
// (config/symbols.csv); a named global folds to the absolute
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

#include "layout.h"
// Object00553e40: one object, reached through the global at 0x0163f0f0.
//
// Merge evidence: EXE code loads the same global as `this` for sub_00553e40
// (152 call sites, for example 0x004844b6), sub_00553e60 (46, for example
// 0x005613ba) and sub_00553e70 (11, for example 0x0057bd60). In function
// 0x0057bcf0 it feeds sub_00553e60 (0x0057bd53) and then sub_00553e70
// (0x0057bd60). Names are hypotheses.
struct Object00553e40 {
    unsigned int field_0;
    unsigned char unknown_04[0x790];
    unsigned int field_794;

    unsigned int sub_00553e40();
    unsigned int sub_00553e60();
    void sub_00553e70(unsigned int value);
};
LAYOUT_OFFSET(Object00553e40, field_0, 0);
LAYOUT_OFFSET(Object00553e40, field_794, 0x794);

// FUNCTION: RBR 0x00553890
unsigned int Object00553890::sub_00553890() { return ++field_4c; }

// FUNCTION: RBR 0x005538a0
void Object005538a0::sub_005538a0(unsigned int value) { field_468 = value; }

// FUNCTION: RBR 0x005538c0
void Object005538c0::sub_005538c0(unsigned int unused) { field_38 = 1; }

// FUNCTION: RBR 0x005538f0
extern "C" int __cdecl sub_005538f0(unsigned int value) {
    switch (value) {
    case 0:
    case 1:
    case 10:
        return 1;
    default:
        return 0;
    }
}

// FUNCTION: RBR 0x00553910
Object00553910::Object00553910(unsigned int a, unsigned int b) {
    field_00 = a;
    field_04 = b;
    field_08 = 0;
}

// FUNCTION: RBR 0x00553930
Object00553930::Object00553930(unsigned int a, unsigned int b, unsigned int c,
                               unsigned int d, unsigned int e) {
    field_00 = a;
    field_04 = b;
    field_08 = c;
    field_0c = d;
    field_10 = e;
    field_14 = gRichardBurnsName;
}

// FUNCTION: RBR 0x00553e40
unsigned int Object00553e40::sub_00553e40() { return field_0; }

// FUNCTION: RBR 0x00553e60
unsigned int Object00553e40::sub_00553e60() { return field_794; }

// FUNCTION: RBR 0x00553e70
void Object00553e40::sub_00553e70(unsigned int value) { field_794 = value; }
