// Object004995a0 constructor: EXE 0x004995a0, 32 bytes. __thiscall with four
// stack words (RET 0x10), ECX-in/EAX-out per the observed prologue.
// Initializes field_00..0x0c from the four arguments in order. No direct
// callers in Ghidra; consistent with dispatch through a vtable such as
// 0x007137b4.

struct Object004995a0 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
    Object004995a0(unsigned int a, unsigned int b, unsigned int c, unsigned int d);
};

Object004995a0::Object004995a0(unsigned int a, unsigned int b, unsigned int c, unsigned int d)
{
    field_00 = a;
    field_04 = b;
    field_08 = c;
    field_0c = d;
}
