// sub_0046e0f0: EXE 0x0046e0f0, 43 bytes. __thiscall member, four dword
// parameters (RET 0x10).
//
// Virtual slot 0x48 of the screen/coordinate class whose dispatch table is
// EXE 0x00710734: the four-value setter used by sub_0046e0d0 and the tail
// call of sub_0046e120. sub_0046e170 reads the same fields back and converts
// each through the virtual slots 0x14c/0x150, so these are coordinate pairs:
// +0x9c/+0xa8 feed X-style slots and +0xa0/+0xa4 feed Y-style slots.

class Object0046e0f0 {
public:
    unsigned char unknown_00[0x9c];
    unsigned int field_9c;
    unsigned int field_a0;
    unsigned int field_a4;
    unsigned int field_a8;

    void sub_0046e0f0(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4);
};

typedef char Object0046e0f0_size[(sizeof(Object0046e0f0) == 0xac) ? 1 : -1];

void Object0046e0f0::sub_0046e0f0(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4) {
    field_9c = a1;
    field_a0 = a2;
    field_a8 = a3;
    field_a4 = a4;
}
