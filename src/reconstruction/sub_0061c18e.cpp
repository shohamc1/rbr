// sub_0061c18e: EXE 0x0061c18e, 76 bytes. MSVC 7.1 (/O2 /Zl) with the
// file-local size pragma selecting the POP ECX cdecl cleanups.
//
// Array-holder destroy member of the family whose sibling at 0x006128b1
// passes (this, 0x14, stride, callback 0x0061d11f) the same way.
// __thiscall with one stack word (RET 4). ECX is the holder; the flags
// byte is the stack word. When bit 1 is set, the element-destructor helper
// sub_00610b82 runs with four stack words (base = this, count = 0xc,
// stride = the dword at this-4, callback 0x006195ed, which frees *(ECX));
// bit 0 then frees the block header at this-4, and the header pointer is
// returned. Otherwise *this is freed, bit 0 frees this itself, and this is
// returned.

#pragma optimize("s", on)

extern "C" void __stdcall sub_00610b82(void* base, unsigned int count,
                                       unsigned int stride, void* callback);
extern "C" void __cdecl sub_006efdd0(void* memory);

class Object0061c18e {
public:
    void* sub_0061c18e(unsigned char flags);
};

void* Object0061c18e::sub_0061c18e(unsigned char flags) {
    if (flags & 2) {
        char* block = (char*)this - 4;
        sub_00610b82(this, 0xc, *(unsigned int*)block, (void*)0x006195ed);
        if (flags & 1)
            sub_006efdd0(block);
        return block;
    }
    sub_006efdd0(*(void**)this);
    if (flags & 1)
        sub_006efdd0((void*)this);
    return (void*)this;
}
