// sub_00545b00: EXE 0x00545b00, 35 bytes. __thiscall member with one dword
// stack argument (RET 0x4), vtable 0x00726524 slot 0: the scalar
// deleting-destructor shape. Calls the base destructor sub_00545b30 with
// ECX = this, then when the flags argument has bit 0 set releases the 0xd8
// -byte object through cdecl sub_0056f190 (manifest source:null; ORs the
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

Object00545b00 *Object00545b00::sub_00545b00(unsigned int flags) {
    ((Object00545b30 *)(void *)this)->sub_00545b30();
    if (flags & 1)
        sub_0056f190(this, 0xd8);
    return this;
}
