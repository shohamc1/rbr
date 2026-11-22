// Four-lane u16 packer producing the 64-bit values compared by sub_004336d0
// (the version-compare family: its caller sub_00432c40 builds values such as
// 0x00050001_08d20190 from four 16-bit components). The reference body packs
// its inputs as EDX:EAX = (DX<<16|AX) : ([esp+4]<<16|CX), i.e. a 64-bit value
// (param2 << 48) | (param1 << 32) | (param4 << 16) | param3 read from the
// EAX, EDX, ECX registers and one stack word.
//
// NONMATCHING, blocked on the calling convention: that input layout is the
// Borland register convention (first three arguments in EAX, EDX, ECX, the
// rest on the stack), which no MSVC 7.1 declaration reproduces — a __fastcall
// 64-bit parameter goes on the stack (verified: @sub_004336b0@12), and no
// MSVC calling convention binds an argument to EAX. Nothing in the EXE
// references this function (no call, jump, or data reference anywhere in the
// image), so the convention cannot be recovered from a caller either; the
// body is likely dead code from a foreign-compiler object. The draft below
// keeps the packing semantics as a plain cdecl function; it compiles to stack
// loads instead of the register reads. Compiled with MSVC 7.1 (/O2 /Zl).

// Three-way compare of two packed 64-bit values passed as four dwords on the
// stack: value A = (a_hi:a_lo) at [esp+8]/[esp+4], value B = (b_hi:b_lo) at
// [esp+0x10]/[esp+0xc]. Returns 1, -1, or 0 for A>B, A<B, A==B. Callers in
// sub_00432c40 (0x0043323d, 0x00433308, 0x00433380, 0x004334fd) push four
// dwords and test the sign of the result (`test eax,eax; jl`). The compared
// values look like four-u16 version numbers; see the packer sub_004336b0.
// Compiled with MSVC 7.1 (/O2 /Zl): the high dwords are compared once and
// both flag senses reused; the low compare ends in SBB.

// File-interface open callback (slot 0 of the callback table built in
// sub_0041f4f0 at 0x0041f501). The interface is a C-style cdecl callback
// table: (open, read, write, tell, seek, close, error, user_data); the first
// parameter of every callback is the table's user_data slot, which the stdio
// implementation ignores. This one maps open() mode bits to CRT mode strings
// and calls fopen: bit0 set with bits0-1 == 1 selects "rb" (0x007767f4),
// bit2 "r+b" (0x007767f0), bit3 "wb" (0x007767ec). The mode strings are
// referenced by their .rdata addresses; the caller at 0x0041f552 passes
// (self, path, 5). Compiled with MSVC 7.1 (/O2 /Zl).
extern "C" {
__declspec(dllimport) void *__cdecl fopen(const char *path, const char *mode);
}

// File-interface read callback (slot 1 of the callback table built in
// sub_0041f4f0 at 0x0041f509). cdecl(self, stream, buffer, size): forwards to
// fread(buffer, 1, size, stream) and discards the result. The first parameter
// is the table's user_data slot, which the stdio implementation ignores.
// Compiled with MSVC 7.1 (/O2 /Zl).
extern "C" {
__declspec(dllimport) unsigned int __cdecl fread(void *buffer,
                                                 unsigned int size,
                                                 unsigned int count,
                                                 void *stream);
}

// File-interface write callback (slot 2 of the callback table built in
// sub_0041f4f0 at 0x0041f511). cdecl(self, stream, buffer, size): forwards to
// fwrite(buffer, 1, size, stream) and discards the result. The first parameter
// is the table's user_data slot, which the stdio implementation ignores.
// Compiled with MSVC 7.1 (/O2 /Zl).
extern "C" {
__declspec(dllimport) unsigned int __cdecl fwrite(const void *buffer,
                                                  unsigned int size,
                                                  unsigned int count,
                                                  void *stream);
}

// File-interface tell callback (slot 3 of the callback table built in
// sub_0041f4f0 at 0x0041f519). cdecl(self, stream): returns ftell(stream)
// unchanged. The first parameter is the table's user_data slot, which the
// stdio implementation ignores. Compiled with MSVC 7.1 (/O2 /Zl).
extern "C" {
__declspec(dllimport) long __cdecl ftell(void *stream);
}

// File-interface seek callback (slot 4 of the callback table built in
// sub_0041f4f0 at 0x0041f521). cdecl(self, stream, offset, origin): validates
// the origin (0, 1, or 2; anything else returns -1 without touching the
// stream), maps it to the identical CRT SEEK_* constant, calls
// fseek(stream, offset, whence), and always returns 0. The first parameter is
// the table's user_data slot, which the stdio implementation ignores. The
// caller at 0x0041f58b passes (self, stream, offset, 0). Compiled with
// MSVC 7.1 (/O2 /Zl).
extern "C" {
__declspec(dllimport) int __cdecl fseek(void *stream, long offset, int origin);
}

// File-interface close callback (slot 5 of the callback table built in
// sub_0041f4f0 at 0x0041f529). cdecl(self, stream): returns fclose(stream)
// unchanged. The first parameter is the table's user_data slot, which the
// stdio implementation ignores. Compiled with MSVC 7.1 (/O2 /Zl).
extern "C" {
__declspec(dllimport) int __cdecl fclose(void *stream);
}

// File-interface error callback (slot 6 of the callback table built in
// sub_0041f4f0 at 0x0041f531). cdecl(self, stream): returns
// stream->field_c & 0x20 without calling the CRT. The layout matches the
// MSVCRT FILE structure, whose _flag field sits at offset 0xc; bit 0x20 is
// _IOERR, so this is an ferror() specialization reading the flag word
// directly. The first parameter is the table's user_data slot, which the
// stdio implementation ignores. Compiled with MSVC 7.1 (/O2 /Zl).
struct StreamFile {
    unsigned char field_0[0xc];  // _ptr, _cnt, _base
    unsigned int field_c;        // _flag
};

// Reset helper for the object family created in sub_00433890: the caller at
// 0x00433928 tail-jumps here with the outer object in EDX (`mov edx,esi;
// pop ebx; jmp 0x00433840`), a custom register input outside the cdecl
// declaration. Compiled as __fastcall with an unused first argument
// so the object arrives in EDX; the ECX register input is dead in the body.
//
// The outer object (EDX) owns an inner object at +0x1c; the inner object
// holds a byte pool at +0x528 whose address is stored into the three list
// heads at +0x44, +0x48, and +0x64 (reset to an empty state, matching the
// allocator pattern in sub_00433890, which allocates 0x1ba8 bytes for it).
// Returns 0 on success, -2 when either object pointer is null.
// Compiled with MSVC 7.1 (/O2 /Zl).
#include "layout.h"

struct Object00433840Inner {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    unsigned int field_c;
    unsigned int field_10;
    unsigned int field_14;
    unsigned int field_18;
    unsigned int field_1c;
    unsigned int field_20;
    unsigned int field_24;
    unsigned int field_28;
    unsigned int field_2c;
    unsigned int field_30;
    unsigned int field_34;
    unsigned char field_38[0x0c];
    void *field_44;
    void *field_48;
    unsigned char field_4c[0x18];
    void *field_64;
    unsigned char field_68[0x4c0];
    unsigned char field_528;
};
typedef char Assert00433840Inner44[((unsigned int)&(((Object00433840Inner*)0)->field_44) == 0x44) ? 1 : -1];
typedef char Assert00433840Inner528[((unsigned int)&(((Object00433840Inner*)0)->field_528) == 0x528) ? 1 : -1];

struct Object00433840Outer {
    unsigned char field_0[0x8];
    unsigned int field_8;
    unsigned char field_c[0x8];
    unsigned int field_14;
    unsigned int field_18;
    Object00433840Inner *field_1c;
};
typedef char Assert00433840Outer1c[((unsigned int)&(((Object00433840Outer*)0)->field_1c) == 0x1c) ? 1 : -1];

// NONMATCHING
// FUNCTION: RBR 0x004336b0
extern "C" unsigned __int64 __cdecl sub_004336b0(unsigned short param1,
                                                 unsigned short param2,
                                                 unsigned short param3,
                                                 unsigned short param4) {
    return ((unsigned __int64)param2 << 48) | ((unsigned __int64)param1 << 32) |
           ((unsigned __int64)param4 << 16) | param3;
}

// FUNCTION: RBR 0x004336d0
extern "C" int __cdecl sub_004336d0(unsigned int a_lo, unsigned int a_hi,
                                    unsigned int b_lo, unsigned int b_hi) {
    if (a_hi > b_hi) return 1;
    else if (a_hi < b_hi) return -1;
    else if (a_lo > b_lo) return 1;
    else return a_lo < b_lo ? -1 : 0;
}

// FUNCTION: RBR 0x00433700
extern "C" void *__cdecl sub_00433700(void *self, const char *path, int flags) {
    void *result = 0;
    const char *mode = 0;
    if ((flags & 3) == 1) {
        mode = "rb";
    } else if (flags & 4) {
        mode = "r+b";
    } else if (flags & 8) {
        mode = "wb";
    }
    if (path != 0 && mode != 0) {
        result = fopen(path, mode);
    }
    return result;
}

// FUNCTION: RBR 0x00433750
extern "C" void __cdecl sub_00433750(void *self, void *stream, void *buffer,
                                     unsigned int size) {
    fread(buffer, 1, size, stream);
}

// FUNCTION: RBR 0x00433770
extern "C" void __cdecl sub_00433770(void *self, void *stream,
                                     const void *buffer, unsigned int size) {
    fwrite(buffer, 1, size, stream);
}

// FUNCTION: RBR 0x00433790
extern "C" long __cdecl sub_00433790(void *self, void *stream) {
    return ftell(stream);
}

// FUNCTION: RBR 0x004337a0
extern "C" int __cdecl sub_004337a0(void *self, void *stream, long offset,
                                    int origin) {
    int whence;
    switch (origin) {
    case 0:
        whence = 0;
        break;
    case 1:
        whence = 1;
        break;
    case 2:
        whence = 2;
        break;
    default:
        return -1;
    }
    fseek(stream, offset, whence);
    return 0;
}

// FUNCTION: RBR 0x004337e0
extern "C" int __cdecl sub_004337e0(void *self, void *stream) {
    return fclose(stream);
}

// FUNCTION: RBR 0x004337f0
extern "C" unsigned int __cdecl sub_004337f0(void *self, StreamFile *stream) {
    return stream->field_c & 0x20;
}

// FUNCTION: RBR 0x00433840
extern "C" int __fastcall sub_00433840(void *unused, Object00433840Outer *outer) {
    if (outer == 0 || outer->field_1c == 0) {
        return -2;
    }
    Object00433840Inner *inner = outer->field_1c;
    inner->field_18 = 0;
    outer->field_14 = 0;
    outer->field_8 = 0;
    outer->field_18 = 0;
    inner->field_0 = 0;
    inner->field_4 = 0;
    inner->field_c = 0;
    inner->field_20 = 0;
    inner->field_24 = 0;
    inner->field_30 = 0;
    inner->field_34 = 0;
    void *pool = &inner->field_528;
    inner->field_64 = pool;
    inner->field_48 = pool;
    inner->field_44 = pool;
    return 0;
}
