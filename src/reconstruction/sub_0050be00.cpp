// sub_0050be00: EXE 0x50be00, 42 bytes. MSVC 7.1 (/O2 /Zl).
//
// thiscall forwarder (two stack arguments, ret 8) from vtable slot at 0x0071a6d8
// (one of the many sibling wrappers sub_0050bd40..sub_0050be30, each in its
// own vtable). When the receiver's field_08 is set, forwards both arguments
// through vtable offset 0xbc (slot 47) of the object at field_0c and
// discards its result; otherwise returns immediately. Both paths return the
// first argument in EAX.

class Callee0050be00 {
public:
    virtual void slot000(void* a, void* b);
    virtual void slot001(void* a, void* b);
    virtual void slot002(void* a, void* b);
    virtual void slot003(void* a, void* b);
    virtual void slot004(void* a, void* b);
    virtual void slot005(void* a, void* b);
    virtual void slot006(void* a, void* b);
    virtual void slot007(void* a, void* b);
    virtual void slot008(void* a, void* b);
    virtual void slot009(void* a, void* b);
    virtual void slot010(void* a, void* b);
    virtual void slot011(void* a, void* b);
    virtual void slot012(void* a, void* b);
    virtual void slot013(void* a, void* b);
    virtual void slot014(void* a, void* b);
    virtual void slot015(void* a, void* b);
    virtual void slot016(void* a, void* b);
    virtual void slot017(void* a, void* b);
    virtual void slot018(void* a, void* b);
    virtual void slot019(void* a, void* b);
    virtual void slot020(void* a, void* b);
    virtual void slot021(void* a, void* b);
    virtual void slot022(void* a, void* b);
    virtual void slot023(void* a, void* b);
    virtual void slot024(void* a, void* b);
    virtual void slot025(void* a, void* b);
    virtual void slot026(void* a, void* b);
    virtual void slot027(void* a, void* b);
    virtual void slot028(void* a, void* b);
    virtual void slot029(void* a, void* b);
    virtual void slot030(void* a, void* b);
    virtual void slot031(void* a, void* b);
    virtual void slot032(void* a, void* b);
    virtual void slot033(void* a, void* b);
    virtual void slot034(void* a, void* b);
    virtual void slot035(void* a, void* b);
    virtual void slot036(void* a, void* b);
    virtual void slot037(void* a, void* b);
    virtual void slot038(void* a, void* b);
    virtual void slot039(void* a, void* b);
    virtual void slot040(void* a, void* b);
    virtual void slot041(void* a, void* b);
    virtual void slot042(void* a, void* b);
    virtual void slot043(void* a, void* b);
    virtual void slot044(void* a, void* b);
    virtual void slot045(void* a, void* b);
    virtual void slot046(void* a, void* b);
    virtual void slot047(void* a, void* b);  // vtable offset 0xbc
};

struct Object0050be00 {
    unsigned char unknown_00[8];
    void* field_08;
    Callee0050be00* field_0c;
    void* sub_0050be00(void* a, void* b);
};

void* Object0050be00::sub_0050be00(void* a, void* b) {
    if (field_08 != 0) {
        field_0c->slot047(a, b);
    }
    return a;
}
