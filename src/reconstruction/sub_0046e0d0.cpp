// sub_0046e0d0: EXE 0x0046e0d0, 28 bytes. __thiscall member, no parameters.
//
// Part of the screen/coordinate class whose dispatch table lives at EXE
// 0x00710734 (set by the SEH constructors at 0x0046f960/0x0046ff80). This
// member forwards four integer fields to the virtual slot 0x48 setter
// (sub_0046e0f0 in that table), which stores them at +0x9c/+0xa0/+0xa8/+0xa4.
// This function itself is not stored in that table; callers reach it directly.
//
// Local dispatch-table model: the virtual slot at byte offset 0x48 needs the
// eighteen preceding slots, declared here as unnamed stubs.

class Object0046e0d0 {
public:
    virtual void vslot00(); virtual void vslot01(); virtual void vslot02();
    virtual void vslot03(); virtual void vslot04(); virtual void vslot05();
    virtual void vslot06(); virtual void vslot07(); virtual void vslot08();
    virtual void vslot09(); virtual void vslot0a(); virtual void vslot0b();
    virtual void vslot0c(); virtual void vslot0d(); virtual void vslot0e();
    virtual void vslot0f(); virtual void vslot10(); virtual void vslot11();
    virtual void vslot48(unsigned int, unsigned int, unsigned int, unsigned int);

    unsigned char unknown_04[0x10];
    unsigned int field_14;
    unsigned int field_18;
    unsigned char unknown_1c[0x78];
    unsigned int field_94;
    unsigned int field_98;

    void sub_0046e0d0();
};

typedef char Object0046e0d0_size[(sizeof(Object0046e0d0) == 0x9c) ? 1 : -1];

void Object0046e0d0::sub_0046e0d0() {
    vslot48(field_98, field_94, field_14, field_18);
}
