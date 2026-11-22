// sub_0040e5d0: EXE 0x0040e5d0, 111 bytes. MSVC 7.1 (/O2 /Zl).
//
// Object0040e570-family member: the same receiver shape that holds the
// inner statistics record at +0x6e4 and the dispatcher object at +0xf4
// (see Object0040e570.h for the class evidence). thiscall, six stack
// arguments, RET 0x18.
//
// Bumps the inner counter at +0x38, adds the sixth argument to the
// accumulator at +0x30, then scans the five thresholds at +0x50 and bumps
// the first bucket counter at +0x64 whose threshold is above the value
// (unsigned compare; first match wins). Forwards all six arguments
// through dispatcher vtable slot 0x148 (index 82) and discards the result.
// The dispatch entry takes its receiver as the first stack argument and
// cleans its own frame (__stdcall member), so the caller pushes the
// receiver object right before the call.
#include "layout.h"

// Pointee of the +0xf4 dispatcher pointer; only slot 82 (vtable +0x148)
// is observed. The other declarations place the entry.
class Callee0040e5d0 {
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
    virtual void slot042();
    virtual void slot043();
    virtual void slot044();
    virtual void slot045();
    virtual void slot046();
    virtual void slot047();
    virtual void slot048();
    virtual void slot049();
    virtual void slot050();
    virtual void slot051();
    virtual void slot052();
    virtual void slot053();
    virtual void slot054();
    virtual void slot055();
    virtual void slot056();
    virtual void slot057();
    virtual void slot058();
    virtual void slot059();
    virtual void slot060();
    virtual void slot061();
    virtual void slot062();
    virtual void slot063();
    virtual void slot064();
    virtual void slot065();
    virtual void slot066();
    virtual void slot067();
    virtual void slot068();
    virtual void slot069();
    virtual void slot070();
    virtual void slot071();
    virtual void slot072();
    virtual void slot073();
    virtual void slot074();
    virtual void slot075();
    virtual void slot076();
    virtual void slot077();
    virtual void slot078();
    virtual void slot079();
    virtual void slot080();
    virtual void slot081();
    virtual void __stdcall slot082(int arg1, int arg2, int arg3, int arg4, int arg5, unsigned int arg6);
};

// Pointee of the +0x6e4 inner pointer for this member.
struct Inner0040e5d0 {
    unsigned char unknown_00[0x30];
    unsigned int field_30;
    unsigned char unknown_34[4];
    unsigned int field_38;
    unsigned char unknown_3c[0x14];
    unsigned int field_50[5];
    unsigned int field_64[5];
};

LAYOUT_OFFSET(Inner0040e5d0, field_30, 0x30);
LAYOUT_OFFSET(Inner0040e5d0, field_38, 0x38);
LAYOUT_OFFSET(Inner0040e5d0, field_50, 0x50);
LAYOUT_OFFSET(Inner0040e5d0, field_64, 0x64);

class Object0040e5d0 {
public:
    unsigned char unknown_00[0xf4];
    Callee0040e5d0* field_f4;
    unsigned char unknown_f8[0x5ec];
    Inner0040e5d0* field_6e4;

    void sub_0040e5d0(int arg1, int arg2, int arg3, int arg4, int arg5, unsigned int arg6);
};

LAYOUT_OFFSET(Object0040e5d0, field_f4, 0xf4);
LAYOUT_OFFSET(Object0040e5d0, field_6e4, 0x6e4);

void Object0040e5d0::sub_0040e5d0(int arg1, int arg2, int arg3, int arg4, int arg5,
                                  unsigned int arg6) {
    field_6e4->field_38++;
    field_6e4->field_30 += arg6;
    for (unsigned int i = 0; i < 5; i++) {
        if (field_6e4->field_50[i] > arg6) {
            field_6e4->field_64[i]++;
            break;
        }
    }
    field_f4->slot082(arg1, arg2, arg3, arg4, arg5, arg6);
}
