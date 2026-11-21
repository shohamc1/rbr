// sub_0040e7c0: EXE 0x0040e7c0, 60 bytes. MSVC 7.1 (/O2 /Zl).
//
// Object0040e570-family member (inner record at +0x6e4, dispatcher object
// at +0xf4; see Object0040e570.h). thiscall, no stack arguments, bare RET.
// Zeroes the four dwords at inner +0x78..+0x84, then calls dispatcher
// vtable slot 0xa8 (index 42), a __stdcall-style entry that takes its
// receiver as the only stack argument and cleans its own frame.
#include "layout.h"

// Pointee of the +0xf4 dispatcher pointer; only slot 42 (vtable +0xa8)
// is observed. The other declarations place the entry.
class Callee0040e7c0 {
public:
    virtual void slot000();
    virtual void slot001();
    virtual void slot002();
    virtual void slot003();
    virtual void slot004();
    virtual void slot005();
    virtual void slot006();
    virtual void slot007();
    virtual void slot008();
    virtual void slot009();
    virtual void slot010();
    virtual void slot011();
    virtual void slot012();
    virtual void slot013();
    virtual void slot014();
    virtual void slot015();
    virtual void slot016();
    virtual void slot017();
    virtual void slot018();
    virtual void slot019();
    virtual void slot020();
    virtual void slot021();
    virtual void slot022();
    virtual void slot023();
    virtual void slot024();
    virtual void slot025();
    virtual void slot026();
    virtual void slot027();
    virtual void slot028();
    virtual void slot029();
    virtual void slot030();
    virtual void slot031();
    virtual void slot032();
    virtual void slot033();
    virtual void slot034();
    virtual void slot035();
    virtual void slot036();
    virtual void slot037();
    virtual void slot038();
    virtual void slot039();
    virtual void slot040();
    virtual void slot041();
    virtual void __stdcall slot042();
};

// Pointee of the +0x6e4 inner pointer for this member.
struct Inner0040e7c0 {
    unsigned char unknown_00[0x78];
    unsigned int field_78;
    unsigned int field_7c;
    unsigned int field_80;
    unsigned int field_84;
};

LAYOUT_OFFSET(Inner0040e7c0, field_78, 0x78);
LAYOUT_OFFSET(Inner0040e7c0, field_84, 0x84);

class Object0040e7c0 {
public:
    unsigned char unknown_00[0xf4];
    Callee0040e7c0* field_f4;
    unsigned char unknown_f8[0x5ec];
    Inner0040e7c0* field_6e4;

    void sub_0040e7c0();
};

LAYOUT_OFFSET(Object0040e7c0, field_f4, 0xf4);
LAYOUT_OFFSET(Object0040e7c0, field_6e4, 0x6e4);

void Object0040e7c0::sub_0040e7c0() {
    field_6e4->field_78 = 0;
    field_6e4->field_7c = 0;
    field_6e4->field_80 = 0;
    field_6e4->field_84 = 0;
    field_f4->slot042();
}
