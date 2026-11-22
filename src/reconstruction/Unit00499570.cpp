// sub_00499570: __thiscall, one stack word (RET 4).
// field_00 is the buffer end, field_04 the write cursor. When
// cursor + count would pass the end, zero-fills the overflow amount byte by
// byte and advances the cursor. The byte store may alias the cursor field, so
// both statements reload it each iteration.

struct Object00499570 {
    char* field_00;
    char* field_04;
    void sub_00499570(int count);
};

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

// FUNCTION: RBR 0x00499570
void Object00499570::sub_00499570(int count)
{
    int overflow = (field_00 - field_04) + count;
    if (overflow > 0) {
        do {
            *field_04 = 0;
            field_04 = field_04 + 1;
        } while (--overflow);
    }
}

// FUNCTION: RBR 0x004995a0
Object004995a0::Object004995a0(unsigned int a, unsigned int b, unsigned int c, unsigned int d)
{
    field_00 = a;
    field_04 = b;
    field_08 = c;
    field_0c = d;
}
