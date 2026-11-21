// sub_0040e790: EXE 0x0040e790, 45 bytes. MSVC 7.1 (/O2 /Zl).
//
// Object0040e570-family member (dispatcher object at +0xf4; see
// Object0040e570.h). thiscall, six stack arguments, RET 0x18. Pure
// forwarder: reloads each argument from its stack slot and passes all six
// through dispatcher vtable slot 0x6c (index 27), a __stdcall-style entry
// that takes its receiver as the first stack argument and cleans its own
// frame. Result discarded.
#include "layout.h"

// Pointee of the +0xf4 dispatcher pointer; only slot 27 (vtable +0x6c)
// is observed. The other declarations place the entry.
class Callee0040e790 {
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
    virtual void __stdcall slot027(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6);
};

class Object0040e790 {
public:
    unsigned char unknown_00[0xf4];
    Callee0040e790* field_f4;

    void sub_0040e790(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6);
};

LAYOUT_OFFSET(Object0040e790, field_f4, 0xf4);

void Object0040e790::sub_0040e790(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6) {
    field_f4->slot027(arg1, arg2, arg3, arg4, arg5, arg6);
}
