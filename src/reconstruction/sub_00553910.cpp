// sub_00553910: EXE 0x00553910, 25 bytes. __thiscall constructor with two
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

Object00553910::Object00553910(unsigned int a, unsigned int b) {
    field_00 = a;
    field_04 = b;
    field_08 = 0;
}
