// sub_0050bca0: MSVC 7.1 (/O2 /Zl).
//
// __cdecl delete-helper: when the pointer argument is non-null, calls vtable
// slot 0 (a scalar deleting destructor, flags = 1) on the object and lets the
// callee clean its own argument; otherwise returns immediately. The caller
// cleans the single cdecl argument (bare RET).

class Object0050bca0 {
public:
    virtual void sub(unsigned int flags); // vtable slot 0
};

// sub_0050bd40: MSVC 7.1 (/O2 /Zl).
//
// thiscall forwarder (two stack arguments, ret 8) from vtable slot at 0x0071a6c8
// (one of the many sibling wrappers sub_0050bd40..sub_0050be30, each in its
// own vtable). When the receiver's field_08 is set, forwards both arguments
// through vtable offset 0x9c (slot 39) of the object at field_0c and
// discards its result; otherwise returns immediately. Both paths return the
// first argument in EAX.

class Callee0050bd40 {
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
    virtual void slot039(void* a, void* b);  // vtable offset 0x9c
};

struct Object0050bd40 {
    unsigned char unknown_00[8];
    void* field_08;
    Callee0050bd40* field_0c;
    void* sub_0050bd40(void* a, void* b);
};

// sub_0050bd70: MSVC 7.1 (/O2 /Zl).
//
// thiscall forwarder (two stack arguments, ret 8) from vtable slot at 0x0071a6cc
// (one of the many sibling wrappers sub_0050bd40..sub_0050be30, each in its
// own vtable). When the receiver's field_08 is set, forwards both arguments
// through vtable offset 0xa0 (slot 40) of the object at field_0c and
// discards its result; otherwise returns immediately. Both paths return the
// first argument in EAX.

class Callee0050bd70 {
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
    virtual void slot040(void* a, void* b);  // vtable offset 0xa0
};

struct Object0050bd70 {
    unsigned char unknown_00[8];
    void* field_08;
    Callee0050bd70* field_0c;
    void* sub_0050bd70(void* a, void* b);
};

// sub_0050bda0: MSVC 7.1 (/O2 /Zl).
//
// thiscall forwarder (two stack arguments, ret 8) from vtable slot at 0x0071a6d0
// (one of the many sibling wrappers sub_0050bd40..sub_0050be30, each in its
// own vtable). When the receiver's field_08 is set, forwards both arguments
// through vtable offset 0xac (slot 43) of the object at field_0c and
// discards its result; otherwise returns immediately. Both paths return the
// first argument in EAX.

class Callee0050bda0 {
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
    virtual void slot043(void* a, void* b);  // vtable offset 0xac
};

struct Object0050bda0 {
    unsigned char unknown_00[8];
    void* field_08;
    Callee0050bda0* field_0c;
    void* sub_0050bda0(void* a, void* b);
};

// sub_0050bdd0: MSVC 7.1 (/O2 /Zl).
//
// thiscall forwarder (two stack arguments, ret 8) from vtable slot at 0x0071a6d4
// (one of the many sibling wrappers sub_0050bd40..sub_0050be30, each in its
// own vtable). When the receiver's field_08 is set, forwards both arguments
// through vtable offset 0xb0 (slot 44) of the object at field_0c and
// discards its result; otherwise returns immediately. Both paths return the
// first argument in EAX.

class Callee0050bdd0 {
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
    virtual void slot044(void* a, void* b);  // vtable offset 0xb0
};

struct Object0050bdd0 {
    unsigned char unknown_00[8];
    void* field_08;
    Callee0050bdd0* field_0c;
    void* sub_0050bdd0(void* a, void* b);
};

// sub_0050be00: MSVC 7.1 (/O2 /Zl).
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

// sub_0050be30: MSVC 7.1 (/O2 /Zl).
//
// thiscall forwarder (two stack arguments, ret 8) from vtable slot at 0x0071a6dc
// (one of the many sibling wrappers sub_0050bd40..sub_0050be30, each in its
// own vtable). When the receiver's field_08 is set, forwards both arguments
// through vtable offset 0xc0 (slot 48) of the object at field_0c and
// discards its result; otherwise returns immediately. Both paths return the
// first argument in EAX.

class Callee0050be30 {
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
    virtual void slot047(void* a, void* b);
    virtual void slot048(void* a, void* b);  // vtable offset 0xc0
};

struct Object0050be30 {
    unsigned char unknown_00[8];
    void* field_08;
    Callee0050be30* field_0c;
    void* sub_0050be30(void* a, void* b);
};

// FUNCTION: RBR 0x0050bca0
extern "C" void __cdecl sub_0050bca0(Object0050bca0* object) {
    if (object != 0) {
        object->sub(1);
    }
}

// FUNCTION: RBR 0x0050bd40
void* Object0050bd40::sub_0050bd40(void* a, void* b) {
    if (field_08 != 0) {
        field_0c->slot039(a, b);
    }
    return a;
}

// FUNCTION: RBR 0x0050bd70
void* Object0050bd70::sub_0050bd70(void* a, void* b) {
    if (field_08 != 0) {
        field_0c->slot040(a, b);
    }
    return a;
}

// FUNCTION: RBR 0x0050bda0
void* Object0050bda0::sub_0050bda0(void* a, void* b) {
    if (field_08 != 0) {
        field_0c->slot043(a, b);
    }
    return a;
}

// FUNCTION: RBR 0x0050bdd0
void* Object0050bdd0::sub_0050bdd0(void* a, void* b) {
    if (field_08 != 0) {
        field_0c->slot044(a, b);
    }
    return a;
}

// FUNCTION: RBR 0x0050be00
void* Object0050be00::sub_0050be00(void* a, void* b) {
    if (field_08 != 0) {
        field_0c->slot047(a, b);
    }
    return a;
}

// FUNCTION: RBR 0x0050be30
void* Object0050be30::sub_0050be30(void* a, void* b) {
    if (field_08 != 0) {
        field_0c->slot048(a, b);
    }
    return a;
}
