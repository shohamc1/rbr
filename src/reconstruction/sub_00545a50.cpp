// sub_00545a50: EXE 0x00545a50, 16 bytes. __thiscall constructor, no stack
// arguments, ECX-in/EAX-out (MOV EAX,ECX prologue). Seeds field_00 with the
// vtable at EXE 0x007264e4 (slot 0 = 0x00545a80, deleting-destructor shape)
// and zeroes field_04. The vtable is mirrored by sub_00547690's constructor
// (vtable 0x007265ac, overrides at +0 and +0xc), so this is the base
// constructor of that hierarchy; no direct EXE caller is indexed.
// Raw-vtable idiom of the matched sub_004495b0 (vtable 0x0070e994).
// Compiled with MSVC 7.1 (/O2 /Zl).

struct Object00545a50 {
    unsigned int vtable;
    unsigned int field_04;
    Object00545a50();
};

Object00545a50::Object00545a50() {
    vtable = 0x7264e4;
    field_04 = 0;
}
