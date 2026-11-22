// sub_00545a50: __thiscall constructor, no stack
// arguments, ECX-in/EAX-out (MOV EAX,ECX prologue). Seeds field_00 with the
// vtable at EXE 0x007264e4 (slot 0 = 0x00545a80, deleting-destructor shape)
// and zeroes field_04. The vtable is mirrored by sub_00547690's constructor
// (vtable 0x007265ac, overrides at +0 and +0xc), so this is the base
// constructor of that hierarchy; no direct EXE caller is indexed.
// Raw-vtable idiom of the matched sub_004495b0 (vtable 0x0070e994).
// Compiled with MSVC 7.1 (/O2 /Zl).

extern "C" const void* const gVtable007264e4[]; // EXE .rdata 0x007264e4

struct Object00545a50 {
    unsigned int vtable;
    unsigned int field_04;
    Object00545a50();
};

// sub_00545ad0: __thiscall setter with one dword
// stack argument (RET 0x4): stores it at +0xd4. sub_00545af0 loads the same
// field and tail-jumps through it, so the value is a callback; both act on
// the class whose vtable sub_00545b30 seeds at 0x00726524 (there sub_00545af0
// sits in slot 0x34). Compiled with MSVC 7.1 (/O2 /Zl).

class Object00545ad0 {
public:
    unsigned char unknown_00[0xd4];
    unsigned int field_d4;
    void sub_00545ad0(unsigned int callback);
};

typedef char Object00545ad0_offset[((unsigned int)&(((Object00545ad0 *)0)->field_d4) == 0xd4) ? 1 : -1];

// sub_00545af0: __thiscall member, no stack
// arguments (vtable 0x00726524 slot 0x34; sub_00545ad0 stores the field).
// Loads the cdecl callback at +0xd4 and, when nonzero, tail-jumps through
// it: MOV EAX,[ECX+0xd4]; TEST EAX,EAX; JZ ret; JMP EAX. The callback local
// keeps the guard-and-tail-jump shape. Compiled with MSVC 7.1 (/O2 /Zl).

class Object00545af0 {
public:
    unsigned char unknown_00[0xd4];
    void (__cdecl *field_d4)();
    void sub_00545af0();
};

typedef char Object00545af0_offset[((unsigned int)&(((Object00545af0 *)0)->field_d4) == 0xd4) ? 1 : -1];

// sub_00545b00: __thiscall member with one dword
// stack argument (RET 0x4), vtable 0x00726524 slot 0: the scalar
// deleting-destructor shape. Calls the base destructor sub_00545b30 with
// ECX = this, then when the flags argument has bit 0 set releases the 0xd8
// -byte object through cdecl sub_0056f190 (stub in config/symbols.csv; ORs the
// sign bit at [ptr-4] then tails into 0x0056eee0 — the debug-heap free
// family), and returns this in EAX. Written as a plain named member, not a
// C++ destructor, to avoid the ??_G COMDAT the one-function rule rejects.
// Compiled with MSVC 7.1 (/O2 /Zl).

class Object00545b30 {
public:
    void sub_00545b30();
};

extern "C" void __cdecl sub_0056f190(void *block, unsigned int size);

class Object00545b00 {
public:
    Object00545b00 *sub_00545b00(unsigned int flags);
};

// FUNCTION: RBR 0x00545a50
Object00545a50::Object00545a50() {
    vtable = (unsigned int)gVtable007264e4;
    field_04 = 0;
}

// FUNCTION: RBR 0x00545ad0
void Object00545ad0::sub_00545ad0(unsigned int callback) {
    field_d4 = callback;
}

// FUNCTION: RBR 0x00545af0
void Object00545af0::sub_00545af0() {
    void (__cdecl *callback)() = field_d4;
    if (callback)
        callback();
}

// FUNCTION: RBR 0x00545b00
Object00545b00 *Object00545b00::sub_00545b00(unsigned int flags) {
    ((Object00545b30 *)(void *)this)->sub_00545b30();
    if (flags & 1)
        sub_0056f190(this, 0xd8);
    return this;
}
