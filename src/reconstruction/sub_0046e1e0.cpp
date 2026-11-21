// sub_0046e1e0: EXE 0x0046e1e0, 51 bytes. __thiscall member, four unsigned
// int* out-parameters (RET 0x10).
//
// Virtual slot 0x40 of the screen/coordinate class whose dispatch table is
// EXE 0x00710734. Writes the four fields +0x84/+0x88/+0x8c/+0x90 through the
// pointer arguments. sub_0046e0d0 forwards +0x14/+0x18 into the slot 0x48
// setter pair (+0xa8/+0xa4), so +0x84..+0x90 hold a second independent
// coordinate quad that callers read back through this getter.

class Object0046e1e0 {
public:
    unsigned char unknown_00[0x84];
    unsigned int field_84;
    unsigned int field_88;
    unsigned int field_8c;
    unsigned int field_90;

    void sub_0046e1e0(unsigned int* o1, unsigned int* o2, unsigned int* o3, unsigned int* o4);
};

typedef char Object0046e1e0_size[(sizeof(Object0046e1e0) == 0x94) ? 1 : -1];

void Object0046e1e0::sub_0046e1e0(unsigned int* o1, unsigned int* o2, unsigned int* o3, unsigned int* o4) {
    *o1 = field_84;
    *o2 = field_88;
    *o3 = field_8c;
    *o4 = field_90;
}
