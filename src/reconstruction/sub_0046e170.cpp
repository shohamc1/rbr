// sub_0046e170: EXE 0x0046e170, 99 bytes. __thiscall member, four float*
// out-parameters (RET 0x10).
//
// Virtual slot 0x54 of the screen/coordinate class whose dispatch table is
// EXE 0x00710734. Reads back the four coordinates stored by sub_0046e0f0
// (+0x9c/+0xa0/+0xa8/+0xa4) and converts each to a float through the
// int-to-float virtual slots 0x14c/0x150 (0x49d0b0/0x49d0d0 in the table:
// ratio against +0x14/+0x18 with a 360.0 wrap, scaled by 64.0/480.0).
// Each converted value returns on the x87 stack and is stored with FSTP
// through the matching pointer argument.
//
// Local dispatch-table model: stub virtuals pad the used slots to their byte
// offsets (0x14c = index 83, 0x150 = index 84).

class Object0046e170 {
public:
    virtual void vslot00(); virtual void vslot01(); virtual void vslot02();
    virtual void vslot03(); virtual void vslot04(); virtual void vslot05();
    virtual void vslot06(); virtual void vslot07(); virtual void vslot08();
    virtual void vslot09(); virtual void vslot0a(); virtual void vslot0b();
    virtual void vslot0c(); virtual void vslot0d(); virtual void vslot0e();
    virtual void vslot0f(); virtual void vslot10(); virtual void vslot11();
    virtual void vslot12(); virtual void vslot13(); virtual void vslot14();
    virtual void vslot15(); virtual void vslot16(); virtual void vslot17();
    virtual void vslot18(); virtual void vslot19(); virtual void vslot1a();
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
    virtual float vslot150(int);

    unsigned char unknown_04[0x98];
    unsigned int field_9c;
    unsigned int field_a0;
    unsigned int field_a4;
    unsigned int field_a8;

    void sub_0046e170(float* o1, float* o2, float* o3, float* o4);
};

typedef char Object0046e170_size[(sizeof(Object0046e170) == 0xac) ? 1 : -1];

void Object0046e170::sub_0046e170(float* o1, float* o2, float* o3, float* o4) {
    *o1 = vslot14c(field_9c);
    *o2 = vslot150(field_a0);
    *o3 = vslot14c(field_a8);
    *o4 = vslot150(field_a4);
}
