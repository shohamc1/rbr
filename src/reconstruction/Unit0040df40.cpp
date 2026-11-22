#include "Object0040e570.h"

// sub_0040e5d0: MSVC 7.1 (/O2 /Zl).
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

// sub_0040e640: MSVC 7.1 (/O2 /Zl).
//
// Object0040e570-family member (inner statistics record at +0x6e4,
// dispatcher object at +0xf4; see Object0040e570.h). thiscall, three stack
// arguments, RET 0xc. Same shape as sub_0040e5d0 with the +0x34 counter
// and dispatcher vtable slot 0x144 (index 81).
#include "layout.h"

// Pointee of the +0xf4 dispatcher pointer; only slot 81 (vtable +0x144)
// is observed. The other declarations place the entry.
class Callee0040e640 {
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
    virtual void __stdcall slot081(int arg1, int arg2, unsigned int arg3);
};

// Pointee of the +0x6e4 inner pointer for this member.
struct Inner0040e640 {
    unsigned char unknown_00[0x30];
    unsigned int field_30;
    unsigned int field_34;
    unsigned int field_38;
    unsigned char unknown_3c[0x14];
    unsigned int field_50[5];
    unsigned int field_64[5];
};

LAYOUT_OFFSET(Inner0040e640, field_30, 0x30);
LAYOUT_OFFSET(Inner0040e640, field_34, 0x34);
LAYOUT_OFFSET(Inner0040e640, field_38, 0x38);
LAYOUT_OFFSET(Inner0040e640, field_50, 0x50);
LAYOUT_OFFSET(Inner0040e640, field_64, 0x64);

class Object0040e640 {
public:
    unsigned char unknown_00[0xf4];
    Callee0040e640* field_f4;
    unsigned char unknown_f8[0x5ec];
    Inner0040e640* field_6e4;

    void sub_0040e640(int arg1, int arg2, unsigned int arg3);
};

LAYOUT_OFFSET(Object0040e640, field_f4, 0xf4);
LAYOUT_OFFSET(Object0040e640, field_6e4, 0x6e4);

// sub_0040e6a0: MSVC 7.1 (/O2 /Zl).
//
// Object0040e570-family member (inner record at +0x6e4, dispatcher object
// at +0xf4; see Object0040e570.h). thiscall, one stack argument, RET 4,
// returns EAX from the dispatch call.
//
// If the inner change flag at +0x8 is set, it is cleared and the value at
// +0x0 is stored (argument), the +0x3c counter bumps, and the argument is
// forwarded through dispatcher vtable slot 0x170 (index 92). If the flag
// is clear and the stored value already equals the argument, only the
// +0x48 counter bumps and the function returns zero.
#include "layout.h"

// Pointee of the +0xf4 dispatcher pointer; only slot 92 (vtable +0x170)
// is observed. The other declarations place the entry.
class Callee0040e6a0 {
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
    virtual void slot089();
    virtual void slot090();
    virtual void slot091();
    virtual int __stdcall slot092(int value);
};

// Pointee of the +0x6e4 inner pointer for this member.
struct Inner0040e6a0 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned char field_08;
    unsigned char unknown_09[0x33];
    unsigned int field_3c;
    unsigned char unknown_40[8];
    unsigned int field_48;
};

LAYOUT_OFFSET(Inner0040e6a0, field_00, 0x00);
LAYOUT_OFFSET(Inner0040e6a0, field_08, 0x08);
LAYOUT_OFFSET(Inner0040e6a0, field_3c, 0x3c);
LAYOUT_OFFSET(Inner0040e6a0, field_48, 0x48);

class Object0040e6a0 {
public:
    unsigned char unknown_00[0xf4];
    Callee0040e6a0* field_f4;
    unsigned char unknown_f8[0x5ec];
    Inner0040e6a0* field_6e4;

    int sub_0040e6a0(int value);
};

LAYOUT_OFFSET(Object0040e6a0, field_f4, 0xf4);
LAYOUT_OFFSET(Object0040e6a0, field_6e4, 0x6e4);

// sub_0040e6f0: MSVC 7.1 (/O2 /Zl).
//
// Object0040e570-family member (inner record at +0x6e4, dispatcher object
// at +0xf4; see Object0040e570.h). thiscall, one stack argument, RET 4,
// returns EAX from the dispatch call. Mirror of sub_0040e6a0 with the
// +0x9 flag, the +0x4 stored value, the +0x40/+0x4c counters, and
// dispatcher vtable slot 0x1ac (index 107).
#include "layout.h"

// Pointee of the +0xf4 dispatcher pointer; only slot 107 (vtable +0x1ac)
// is observed. The other declarations place the entry.
class Callee0040e6f0 {
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
    virtual void slot089();
    virtual void slot090();
    virtual void slot091();
    virtual void slot092();
    virtual void slot093();
    virtual void slot094();
    virtual void slot095();
    virtual void slot096();
    virtual void slot097();
    virtual void slot098();
    virtual void slot099();
    virtual void slot100();
    virtual void slot101();
    virtual void slot102();
    virtual void slot103();
    virtual void slot104();
    virtual void slot105();
    virtual void slot106();
    virtual int __stdcall slot107(int value);
};

// Pointee of the +0x6e4 inner pointer for this member.
struct Inner0040e6f0 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned char field_08;
    unsigned char field_09;
    unsigned char unknown_0a[0x36];
    unsigned int field_40;
    unsigned char unknown_44[8];
    unsigned int field_4c;
};

LAYOUT_OFFSET(Inner0040e6f0, field_04, 0x04);
LAYOUT_OFFSET(Inner0040e6f0, field_09, 0x09);
LAYOUT_OFFSET(Inner0040e6f0, field_40, 0x40);
LAYOUT_OFFSET(Inner0040e6f0, field_4c, 0x4c);

class Object0040e6f0 {
public:
    unsigned char unknown_00[0xf4];
    Callee0040e6f0* field_f4;
    unsigned char unknown_f8[0x5ec];
    Inner0040e6f0* field_6e4;

    int sub_0040e6f0(int value);
};

LAYOUT_OFFSET(Object0040e6f0, field_f4, 0xf4);
LAYOUT_OFFSET(Object0040e6f0, field_6e4, 0x6e4);

#include "Object0040e570.h"

extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)

// sub_0040e790: MSVC 7.1 (/O2 /Zl).
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

// sub_0040e7c0: MSVC 7.1 (/O2 /Zl).
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

// sub_0040e800: MSVC 7.1 (/O2 /Zl).
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

// sub_0040e880: MSVC 7.1 (/O2 /Zl).
//
// Object0040e570-family member (inner record at +0x6e4, dispatcher object
// at +0xf4; see Object0040e570.h). thiscall, one stack argument, RET 4.
//
// Bumps the inner counter at +0x78. A zero argument clears the pending
// pair at +0x8c/+0x88 and returns zero. If the pending value at +0x8c
// equals the argument, bumps the +0x80 repeat counter and returns zero.
// Otherwise stores the argument at +0x8c, clears +0x88, and forwards the
// argument through dispatcher vtable slot 0x15c (index 87), a
// __stdcall-style entry that takes its receiver as the first stack
// argument; its result is the return value.
#include "layout.h"

// Pointee of the +0xf4 dispatcher pointer; only slot 87 (vtable +0x15c)
// is observed. The other declarations place the entry.
class Callee0040e880 {
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
    virtual int __stdcall slot087(int value);
};

// Pointee of the +0x6e4 inner pointer for this member.
struct Inner0040e880 {
    unsigned char unknown_00[0x78];
    unsigned int field_78;
    unsigned int field_7c;
    unsigned int field_80;
    unsigned int field_84;
    unsigned int field_88;
    unsigned int field_8c;
};

LAYOUT_OFFSET(Inner0040e880, field_78, 0x78);
LAYOUT_OFFSET(Inner0040e880, field_80, 0x80);
LAYOUT_OFFSET(Inner0040e880, field_88, 0x88);
LAYOUT_OFFSET(Inner0040e880, field_8c, 0x8c);

class Object0040e880 {
public:
    unsigned char unknown_00[0xf4];
    Callee0040e880* field_f4;
    unsigned char unknown_f8[0x5ec];
    Inner0040e880* field_6e4;

    int sub_0040e880(int value);
};

LAYOUT_OFFSET(Object0040e880, field_f4, 0xf4);
LAYOUT_OFFSET(Object0040e880, field_6e4, 0x6e4);

// sub_0040e960: MSVC 7.1 (/O2 /Zl).
//
// Object0040e570-family member (inner record at +0x6e4; see
// Object0040e570.h). thiscall, no stack arguments, bare RET.
//
// Sets the inner change flags at +0x8 and +0x9, then, through the global
// pointer at 0x016e304c (beside the FILE* global at 0x016e3044), loads
// the pointer at +8 of the pointee and zeroes the dwords at +0x2c and
// +0x30 of that record. These are the same flags sub_0040e6a0 and
// sub_0040e6f0 test and clear.
#include "layout.h"

// Pointee of the +0x6e4 inner pointer for this member.
struct Inner0040e960 {
    unsigned char unknown_00[8];
    unsigned char field_08;
    unsigned char field_09;
};

LAYOUT_OFFSET(Inner0040e960, field_08, 0x08);
LAYOUT_OFFSET(Inner0040e960, field_09, 0x09);

// Pointee of the global at 0x016e304c.
struct Target0040e960;

struct Holder0040e960 {
    unsigned char unknown_00[8];
    Target0040e960* field_08;
};

LAYOUT_OFFSET(Holder0040e960, field_08, 0x08);

// Pointee of Holder0040e960::field_08.
struct Target0040e960 {
    unsigned char unknown_00[0x2c];
    unsigned int field_2c;
    unsigned int field_30;
};

LAYOUT_OFFSET(Target0040e960, field_2c, 0x2c);
LAYOUT_OFFSET(Target0040e960, field_30, 0x30);

extern "C" Holder0040e960* gHolder016e304c; // EXE .data 0x016e304c, beside the FILE* at 0x016e3044

class Object0040e960 {
public:
    unsigned char unknown_00[0x6e4];
    Inner0040e960* field_6e4;

    void sub_0040e960();
};

LAYOUT_OFFSET(Object0040e960, field_6e4, 0x6e4);

// NONMATCHING (one instruction position): the chained flag assignment
// reproduces every instruction and encoding, including the EDX inner load,
// the shared `mov al,1`, the ECX reload, and the 8b 15 global load, but the
// reference hoists `mov edx,[0x016e304c]` above the second flag store
// (`mov [ecx+9],al`); this /O2 build emits it one instruction later. The
// hoist needs a source shape that both sequences the load between the
// reload and the store and keeps the flag value flowing to the second
// store (which is what pins the constant in AL and the inner load in EDX).
// Plain two-statement forms sequence the load correctly but recolor to
// eax/dl/a1; see analysis/notes.md.

// Object0040f9f0 members and the finalizer of its reader block, grouped as one
// module: sub_0040fa60 passes the block that sub_0040f9f0 allocates to
// sub_0040fab0 and then to the operator delete thunk sub_006efdd0.
// The class is declared in Object0040f9f0.h. Compiled with MSVC 7.1 (/O2 /Zl).
#include "Object0040f9f0.h"

extern "C" void* __cdecl sub_006efa32(unsigned int size);
extern "C" void* __stdcall sub_0040f420(void* block);
extern "C" void __stdcall sub_0040fab0(void* block);
extern "C" void __cdecl sub_006efdd0(void* block);

// Nonmatching draft for sub_0040f9f0 (99 bytes).
//
// The reference installs a C++ exception frame before the branch:
//   push -1; push 0x70617b; mov eax, fs:[0]; push eax; mov fs:[0], esp
// sets the EH state to 0 after the allocation, and restores fs:[0] and
// esp on both exits (62 frame bytes in total). EXE 0x70617b is a
// mov eax, 0x79b5f0 / jmp __CxxFrameHandler thunk, so the original frame
// is C++ EH, not SEH3. Reproducing it needs the catch funclet, the
// funcinfo data, and the __CxxFrameHandler import, which this checker
// cannot yet verify. This draft omits the frame; its 37 remaining bytes
// match the reference instruction for instruction (the two call/jump
// displacements are position-dependent).

// FUNCTION: RBR 0x0040df40
unsigned int Object0040e570::sub_0040df40() { return field_f4; }

// FUNCTION: RBR 0x0040e570
unsigned int Object0040e570::sub_0040e570() { return field_e4; }

// FUNCTION: RBR 0x0040e5d0
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

// FUNCTION: RBR 0x0040e640
void Object0040e640::sub_0040e640(int arg1, int arg2, unsigned int arg3) {
    field_6e4->field_34++;
    field_6e4->field_30 += arg3;
    for (unsigned int i = 0; i < 5; i++) {
        if (field_6e4->field_50[i] > arg3) {
            field_6e4->field_64[i]++;
            break;
        }
    }
    field_f4->slot081(arg1, arg2, arg3);
}

// FUNCTION: RBR 0x0040e6a0
int Object0040e6a0::sub_0040e6a0(int value) {
    if (field_6e4->field_08) {
        field_6e4->field_08 = 0;
    } else if (field_6e4->field_00 == value) {
        field_6e4->field_48++;
        return 0;
    }
    field_6e4->field_00 = value;
    field_6e4->field_3c++;
    return field_f4->slot092(value);
}

// FUNCTION: RBR 0x0040e6f0
int Object0040e6f0::sub_0040e6f0(int value) {
    if (field_6e4->field_09) {
        field_6e4->field_09 = 0;
    } else if (field_6e4->field_04 == value) {
        field_6e4->field_4c++;
        return 0;
    }
    field_6e4->field_04 = value;
    field_6e4->field_40++;
    return field_f4->slot107(value);
}

// FUNCTION: RBR 0x0040e740
void Object0040e570::sub_0040e740(unsigned char value) { field_6e0 = value; }

// FUNCTION: RBR 0x0040e750
void Object0040e570::sub_0040e750(unsigned char value) { field_6e1 = value; }

// FUNCTION: RBR 0x0040e790
void Object0040e790::sub_0040e790(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6) {
    field_f4->slot027(arg1, arg2, arg3, arg4, arg5, arg6);
}

// FUNCTION: RBR 0x0040e7c0
void Object0040e7c0::sub_0040e7c0() {
    field_6e4->field_78 = 0;
    field_6e4->field_7c = 0;
    field_6e4->field_80 = 0;
    field_6e4->field_84 = 0;
    field_f4->slot042();
}

// FUNCTION: RBR 0x0040e800
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

// FUNCTION: RBR 0x0040e880
int Object0040e880::sub_0040e880(int value) {
    field_6e4->field_78++;
    if (value == 0) {
        field_6e4->field_8c = 0;
        field_6e4->field_88 = 0;
        return 0;
    }
    if (field_6e4->field_8c == value) {
        ++field_6e4->field_80;
        return 0;
    }
    field_6e4->field_8c = value;
    field_6e4->field_88 = 0;
    return field_f4->slot087(value);
}

// Copies eight dwords with forward REP MOVSD; see analysis/notes.md.
// FUNCTION: RBR 0x0040e900
void Object0040e570::sub_0040e900(const unsigned int* values) { memcpy(field_6c0, values, 32); }

// FUNCTION: RBR 0x0040e920
unsigned int Object0040e570::sub_0040e920() { return field_240; }

// FUNCTION: RBR 0x0040e930
unsigned int Object0040e570::sub_0040e930() { return field_244; }

// NONMATCHING
// FUNCTION: RBR 0x0040e960
void Object0040e960::sub_0040e960() {
    field_6e4->field_09 = (field_6e4->field_08 = 1);
    Holder0040e960* holder = gHolder016e304c;
    Target0040e960* target = holder->field_08;
    target->field_2c = 0;
    target->field_30 = 0;
}

// FUNCTION: RBR 0x0040e9e0
unsigned char* Object0040e570::sub_0040e9e0() { return &field_2d6; }

// FUNCTION: RBR 0x0040ea00
void Object0040e570::sub_0040ea00() {
    Inner0040ea00* value = inner;
    value->field_38 = 0;
    value->field_34 = 0;
    value->field_30 = 0;
    value->field_3c = 0;
    value->field_40 = 0;
    value->field_44 = 0;
    value->field_48 = 0;
    value->field_4c = 0;
    value->field_64 = 0;
    value->field_68 = 0;
    value->field_6c = 0;
    value->field_70 = 0;
    value->field_74 = 0;
}

// FUNCTION: RBR 0x0040ea30
unsigned int Object0040e570::sub_0040ea30() { return field_a0; }

// FUNCTION: RBR 0x0040eb90
unsigned char Object0040e570::sub_0040eb90() { return inner->field_10; }

// NONMATCHING
// FUNCTION: RBR 0x0040f9f0
void* Object0040f9f0::sub_0040f9f0() {
    void* block = sub_006efa32(0x40);
    field_00 = block != 0 ? sub_0040f420(block) : 0;
    return this;
}

// FUNCTION: RBR 0x0040fa60
void Object0040f9f0::sub_0040fa60() {
    void* block = field_00;
    if (block != 0) {
        sub_0040fab0(block);
        sub_006efdd0(block);
    }
    field_00 = 0;
}

// Nonmatching draft for sub_0040fab0 (156 bytes at 0x0040fab0).
//
// sub_0040fab0 is the __stdcall finalizer of the ini-reader block that
// Object0040f9f0::sub_0040fa60 releases: the caller runs this finalizer and
// then sub_006efdd0 (the operator delete thunk) on the same block. The pinned
// body ends with RET 4 at 0x0040fb49. The finalizer destroys the block's
// members in reverse declaration order:
//   1. the ini value string at block+0x1c through MSVCP71 import slot
//      0x70c2d8 (CALL [__imp_...], __thiscall);
//   2. the object held at +0x14 by the container at block+0x10:
//      sub_00411bc0(&slot, [object], object), then operator delete on the
//      object, then both hub fields clear;
//   3. the same release pattern for the hub at block+0x04 (object at +0x08).
//
// The reference installs a C++ exception frame first:
//   push -1; push 0x706166; mov eax, fs:[0]; push eax; mov fs:[0], esp
// EXE 0x706166 is a mov eax, 0x79b5cc / jmp __CxxFrameHandler thunk, so this
// is C++ EH. The EH state moves to 1 around the string destructor
// (0x0040fad0) and back to 0 after it (0x0040fae0). The epilogue at
// 0x0040fb2f restores fs:[0] from the stack slot the first sub_00411bc0 call
// wrote; the reference reuses the saved-ECX slot for that and the dead
// scope-table slot for the second release. Reproducing this needs the unwind
// funclet, the funcinfo data, and checker support for the handler-thunk
// relocation, so this draft omits the frame (the same checker limitation
// recorded for sub_0040f9f0; see analysis/notes.md, Startup subsystem).
//
// Naming the destructor also makes MSVC 7.1 emit
// the local scalar-deleting-dtor closure ??_G...QAEPAXI@Z, so the object holds
// two function symbols; the old per-object checker rejected that. EH frames
// still need checking.

// EXE-internal callees. sub_006efdd0 is the 6-byte operator delete thunk
// (jmp dword ptr [0x70c504], cdecl). sub_00411bc0 consumes three stack arguments
// and the container in EDI, then returns with RET 0xc. It walks node links and
// can reset the head's links to itself. This draft omits the EDI input.
extern "C" void* __stdcall sub_00411bc0(void* slot, void* firstNode, void* lastNode);

// Local container layout; the head pointer is at offset 4.
struct ReleaseHub0040fab0 {
    void* field_00;
    void* object;
    void* field_08;
};

// The reference inlines this release into the finalizer for both hubs.
__forceinline static void releaseHub(ReleaseHub0040fab0* hub) {
    void* released;
    void* firstNode = *static_cast<void**>(hub->object);
    sub_00411bc0(&released, firstNode, hub->object);
    sub_006efdd0(hub->object);
    hub->object = 0;
    hub->field_08 = 0;
}

// The 0x40-byte block from sub_006efa32; member offsets observed here.
// The eight bytes after value are untouched by this finalizer.
struct Block0040fab0 {
    void* field_00;
    ReleaseHub0040fab0 lowHub;   // +0x04
    ReleaseHub0040fab0 highHub;  // +0x10
    std::string value;             // +0x1c
};

// NONMATCHING
// FUNCTION: RBR 0x0040fab0
extern "C" void __stdcall sub_0040fab0(void* block) {
    Block0040fab0* self = static_cast<Block0040fab0*>(block);
    self->value.~basic_string();
    releaseHub(&self->highHub);
    releaseHub(&self->lowHub);
}
