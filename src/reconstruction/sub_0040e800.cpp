// sub_0040e800: EXE 0x0040e800, 113 bytes. MSVC 7.1 (/O2 /Zl).
//
// Object0040e570-family member (inner record at +0x6e4, dispatcher object
// at +0xf4; see Object0040e570.h). thiscall, one stack argument, RET 4,
// returns EAX from the dispatch call.
//
// Bumps the inner counter at +0x7c. If the pending value at +0x88 equals
// the argument and the repeat flag at +0x8c is nonzero, bumps +0x84,
// clears +0x8c, and returns zero. Otherwise stores the argument at +0x88,
// clears +0x8c, and forwards the argument through dispatcher vtable slot
// 0x164 (index 89), a __stdcall-style entry that takes its receiver as
// the first stack argument.
#include "layout.h"

// Pointee of the +0xf4 dispatcher pointer; only slot 89 (vtable +0x164)
// is observed. The other declarations place the entry.
class Callee0040e800 {
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
    virtual void slot082();
    virtual void slot083();
    virtual void slot084();
    virtual void slot085();
    virtual void slot086();
    virtual void slot087();
    virtual void slot088();
    virtual int __stdcall slot089(int value);
};

// Pointee of the +0x6e4 inner pointer for this member.
struct Inner0040e800 {
    unsigned char unknown_00[0x78];
    unsigned int field_78;
    unsigned int field_7c;
    unsigned int field_80;
    unsigned int field_84;
    unsigned int field_88;
    unsigned int field_8c;
};

LAYOUT_OFFSET(Inner0040e800, field_7c, 0x7c);
LAYOUT_OFFSET(Inner0040e800, field_84, 0x84);
LAYOUT_OFFSET(Inner0040e800, field_88, 0x88);
LAYOUT_OFFSET(Inner0040e800, field_8c, 0x8c);

class Object0040e800 {
public:
    unsigned char unknown_00[0xf4];
    Callee0040e800* field_f4;
    unsigned char unknown_f8[0x5ec];
    Inner0040e800* field_6e4;

    int sub_0040e800(int value);
};

LAYOUT_OFFSET(Object0040e800, field_f4, 0xf4);
LAYOUT_OFFSET(Object0040e800, field_6e4, 0x6e4);

int Object0040e800::sub_0040e800(int value) {
    field_6e4->field_7c++;
    if (field_6e4->field_88 == value && field_6e4->field_8c != 0) {
        field_6e4->field_84++;
        field_6e4->field_8c = 0;
        return 0;
    }
    field_6e4->field_88 = value;
    field_6e4->field_8c = 0;
    return field_f4->slot089(value);
}
