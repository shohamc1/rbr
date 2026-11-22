// sub_0046e0d0: __thiscall member, no parameters.
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

// sub_0046e0f0: __thiscall member, four dword
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

// sub_0046e120: __thiscall member, four float
// parameters (RET 0x10).
//
// Virtual slot 0x50 of the screen/coordinate class whose dispatch table is
// EXE 0x00710734. Converts each parameter with the float-to-int virtual
// slots 0x154/0x158 (X/Y style: 0x49d0f0/0x49d120 in the table, ratio
// converters that call _ftol) and feeds the four ints to the slot 0x48
// setter (sub_0046e0f0). The compiled body interleaves the argument pushes:
// each conversion result is pushed and stays on the stack as one of the four
// setter arguments.
//
// Local dispatch-table model: stub virtuals pad the used slots to their byte
// offsets (0x48 = index 18, 0x154 = index 85, 0x158 = index 86).

class Object0046e120 {
public:
    virtual void vslot00(); virtual void vslot01(); virtual void vslot02();
    virtual void vslot03(); virtual void vslot04(); virtual void vslot05();
    virtual void vslot06(); virtual void vslot07(); virtual void vslot08();
    virtual void vslot09(); virtual void vslot0a(); virtual void vslot0b();
    virtual void vslot0c(); virtual void vslot0d(); virtual void vslot0e();
    virtual void vslot0f(); virtual void vslot10(); virtual void vslot11();
    virtual void vslot48(int, int, int, int);
    virtual void vslot13(); virtual void vslot14(); virtual void vslot15();
    virtual void vslot16(); virtual void vslot17(); virtual void vslot18();
    virtual void vslot19(); virtual void vslot1a(); virtual void vslot1b();
    virtual void vslot1c(); virtual void vslot1d(); virtual void vslot1e();
    virtual void vslot1f(); virtual void vslot20(); virtual void vslot21();
    virtual void vslot22(); virtual void vslot23(); virtual void vslot24();
    virtual void vslot25(); virtual void vslot26(); virtual void vslot27();
    virtual void vslot28(); virtual void vslot29(); virtual void vslot2a();
    virtual void vslot2b(); virtual void vslot2c(); virtual void vslot2d();
    virtual void vslot2e(); virtual void vslot2f(); virtual void vslot30();
    virtual void vslot31(); virtual void vslot32(); virtual void vslot33();
    virtual void vslot34(); virtual void vslot35(); virtual void vslot36();
    virtual void vslot37(); virtual void vslot38(); virtual void vslot39();
    virtual void vslot3a(); virtual void vslot3b(); virtual void vslot3c();
    virtual void vslot3d(); virtual void vslot3e(); virtual void vslot3f();
    virtual void vslot40(); virtual void vslot41(); virtual void vslot42();
    virtual void vslot43(); virtual void vslot44(); virtual void vslot45();
    virtual void vslot46(); virtual void vslot47(); virtual void vslot48_2();
    virtual void vslot49(); virtual void vslot4a(); virtual void vslot4b();
    virtual void vslot4c(); virtual void vslot4d(); virtual void vslot4e();
    virtual void vslot4f(); virtual void vslot50(); virtual void vslot51();
    virtual void vslot52(); virtual void vslot53(); virtual void vslot54();
    virtual int vslot154(float);
    virtual int vslot158(float);

    void sub_0046e120(float a1, float a2, float a3, float a4);
};

// sub_0046e170: __thiscall member, four float*
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

// FUNCTION: RBR 0x0046e0d0
void Object0046e0d0::sub_0046e0d0() {
    vslot48(field_98, field_94, field_14, field_18);
}

// FUNCTION: RBR 0x0046e0f0
void Object0046e0f0::sub_0046e0f0(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4) {
    field_9c = a1;
    field_a0 = a2;
    field_a8 = a3;
    field_a4 = a4;
}

// FUNCTION: RBR 0x0046e120
void Object0046e120::sub_0046e120(float a1, float a2, float a3, float a4) {
    vslot48(vslot154(a1), vslot158(a2), vslot154(a3), vslot158(a4));
}

// FUNCTION: RBR 0x0046e170
void Object0046e170::sub_0046e170(float* o1, float* o2, float* o3, float* o4) {
    *o1 = vslot14c(field_9c);
    *o2 = vslot150(field_a0);
    *o3 = vslot14c(field_a8);
    *o4 = vslot150(field_a4);
}
