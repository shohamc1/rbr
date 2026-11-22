// sub_0046e1e0: __thiscall member, four unsigned
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

// sub_0046e230: __thiscall member, one int
// parameter, float return on the x87 stack (RET 0x4).
//
// Virtual slot 0x60 of the screen/coordinate class whose dispatch table is
// EXE 0x00710734. Feeds the int result of virtual slot 0x68 (sub_0046e270 in
// that table: indexed lookup in the container reached from 0x00874618,
// returning its +0x18 field) into the int-to-float converter slot 0x14c and
// returns the float without an intervening store; the x87 value from the
// callee reaches this member's caller directly.

class Object0046e230 {
public:
    virtual void vslot00(); virtual void vslot01(); virtual void vslot02();
    virtual void vslot03(); virtual void vslot04(); virtual void vslot05();
    virtual void vslot06(); virtual void vslot07(); virtual void vslot08();
    virtual void vslot09(); virtual void vslot0a(); virtual void vslot0b();
    virtual void vslot0c(); virtual void vslot0d(); virtual void vslot0e();
    virtual void vslot0f(); virtual void vslot10(); virtual void vslot11();
    virtual void vslot12(); virtual void vslot13(); virtual void vslot14();
    virtual void vslot15(); virtual void vslot16(); virtual void vslot17();
    virtual void vslot18(); virtual void vslot19();
    virtual int vslot68(int);
    virtual void vslot1b(); virtual void vslot1c(); virtual void vslot1d();
    virtual void vslot1e(); virtual void vslot1f(); virtual void vslot20();
    virtual void vslot21(); virtual void vslot22(); virtual void vslot23();
    virtual void vslot24(); virtual void vslot25(); virtual void vslot26();
    virtual void vslot27(); virtual void vslot28(); virtual void vslot29();
    virtual void vslot2a(); virtual void vslot2b(); virtual void vslot2c();
    virtual void vslot2d(); virtual void vslot2e(); virtual void vslot2f();
    virtual void vslot30(); virtual void vslot31(); virtual void vslot32();
    virtual void vslot33(); virtual void vslot34(); virtual void vslot35();
    virtual void vslot36(); virtual void vslot37(); virtual void vslot38();
    virtual void vslot39(); virtual void vslot3a(); virtual void vslot3b();
    virtual void vslot3c(); virtual void vslot3d(); virtual void vslot3e();
    virtual void vslot3f(); virtual void vslot40(); virtual void vslot41();
    virtual void vslot42(); virtual void vslot43(); virtual void vslot44();
    virtual void vslot45(); virtual void vslot46(); virtual void vslot47();
    virtual void vslot48(); virtual void vslot49(); virtual void vslot4a();
    virtual void vslot4b(); virtual void vslot4c(); virtual void vslot4d();
    virtual void vslot4e(); virtual void vslot4f(); virtual void vslot50();
    virtual void vslot51(); virtual void vslot52();
    virtual float vslot14c(int);

    float sub_0046e230(int index);
};

// FUNCTION: RBR 0x0046e1e0
void Object0046e1e0::sub_0046e1e0(unsigned int* o1, unsigned int* o2, unsigned int* o3, unsigned int* o4) {
    *o1 = field_84;
    *o2 = field_88;
    *o3 = field_8c;
    *o4 = field_90;
}

// FUNCTION: RBR 0x0046e230
float Object0046e230::sub_0046e230(int index) {
    return vslot14c(vslot68(index));
}
