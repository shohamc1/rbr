// Object004b0460 member: EXE 0x004b0460, 17 bytes. __thiscall with two
// dword stack arguments (RET 8), ECX receiver, void return.
// Stores them into the fields at +0x10 and +0x14 in argument order. The
// receiver holds at least 0x18 bytes; the first 0x10 bytes match the
// 16-byte list-header family constructed by sub_004b04a0/sub_004b04e0,
// so this is plausibly a pair setter on a class embedding that header.

struct Object004b0460 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
    unsigned int field_10;
    unsigned int field_14;
    void sub_004b0460(unsigned int a, unsigned int b);
};

void Object004b0460::sub_004b0460(unsigned int a, unsigned int b) {
    field_10 = a;
    field_14 = b;
}
