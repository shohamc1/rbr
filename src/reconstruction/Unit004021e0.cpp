// sub_004021e0: MSVC 7.1 (/O2 /Zl).
//
// Scalar deleting destructor (MSVC ??_G shape) referenced from vtable
// 0x0076f3f0. Runs the inlined destructor body — capture the heap block at
// +0x4, restore the vtable word, and release the block through
// GetProcessHeap/HeapFree (IAT 0x0070c188/0x0070c184) — then frees the
// object through the cdecl operator-delete thunk sub_006efdd0 when bit 0 of
// the flags argument is set, and returns this in EAX. RET 4 pops the flags.
// sub_004022f0 is the same body for vtable 0x0076f3e8.

extern "C" const void* const gVtable0076f3f0[]; // EXE .rdata 0x0076f3f0

extern "C" {
__declspec(dllimport) void* __stdcall GetProcessHeap();
__declspec(dllimport) int __stdcall HeapFree(void* heap, unsigned int flags, void* block);
}

// 6-byte cdecl operator-delete thunk at EXE 0x006efdd0 (jmp [0x0070c504]).
extern "C" void __cdecl sub_006efdd0(void* memory);

class Object004021e0 {
public:
    unsigned int field_00;   // vtable slot; 0x0076f3f0 for this class
    void* field_04;          // heap allocation released by the destructor
    void* sub_004021e0(unsigned int flags);
};

// sub_004022f0: MSVC 7.1 (/O2 /Zl).
//
// Scalar deleting destructor (MSVC ??_G shape) referenced from vtable
// 0x0076f3e8; byte-identical to sub_004021e0 except the restored vtable
// constant. Capture the heap block at +0x4, restore the vtable word, release
// the block through GetProcessHeap/HeapFree (IAT 0x0070c188/0x0070c184),
// free the object through the cdecl operator-delete thunk sub_006efdd0 when
// bit 0 of the flags argument is set, and return this in EAX. RET 4.

extern "C" const void* const gVtable0076f3e8[]; // EXE .rdata 0x0076f3e8

extern "C" {
__declspec(dllimport) void* __stdcall GetProcessHeap();
__declspec(dllimport) int __stdcall HeapFree(void* heap, unsigned int flags, void* block);
}

// 6-byte cdecl operator-delete thunk at EXE 0x006efdd0 (jmp [0x0070c504]).
extern "C" void __cdecl sub_006efdd0(void* memory);

class Object004022f0 {
public:
    unsigned int field_00;   // vtable slot; 0x0076f3e8 for this class
    void* field_04;          // heap allocation released by the destructor
    void* sub_004022f0(unsigned int flags);
};

// sub_00402970: MSVC 7.1 (/O2 /Zl).
//
// SSO string buffer getter (c_str style) for the class whose vtable is
// 0x0070c79c: slot 0 is the deleting destructor sub_00402980 and this is
// slot 1. The receiver embeds a 0x10-byte prefix, then an MSVCP71-shaped
// string body: SSO buffer/pointer union at +0x10, length at +0x20, capacity
// at +0x24 (compare msvcp71.h, where the same body sits at offset 0 of
// std::string). Capacity < 0x10 selects the inline buffer at +0x10; otherwise
// the heap pointer stored at +0x10. thiscall, no stack arguments, returns
// the character pointer in EAX.

struct Object00402970 {
    unsigned char field_00[0x10];
    union {
        char field_10_buffer[0x10];
        char* field_10_pointer;
    };
    unsigned int field_20;
    unsigned int field_24;
    const char* sub_00402970() const;
};

// sub_00402980: MSVC 7.1 (/O2 /Zl).
//
// Deleting-destructor body of the class whose vtable is 0x0070c79c (this is
// vtable slot 0; slot 1 is the SSO getter sub_00402970). Calls the destructor
// thunk sub_00402a00 (thiscall: sets the vtable word to 0x0070c79c and jumps
// into the body at 0x004029a0), then frees the object through the CRT thunk
// sub_006efdd0 (jmp [__imp_??3@YAXPAX@Z], operator delete) when bit 0 of the
// flags argument is set. thiscall, one stack argument, returns this in EAX.

extern "C" void __cdecl sub_006efdd0(void* memory);  // 0x006efdd0, operator delete thunk

struct Object00402980 {
    void* sub_00402980(unsigned int flags);
    void sub_00402a00();  // 0x00402a00, destructor thunk (thiscall)
};

// FUNCTION: RBR 0x004021e0
void* Object004021e0::sub_004021e0(unsigned int flags) {
    void* block = field_04;
    field_00 = (unsigned int)gVtable0076f3f0;
    if (block != 0) {
        HeapFree(GetProcessHeap(), 0, block);
    }
    if ((flags & 1) != 0) {
        sub_006efdd0(this);
    }
    return this;
}

// FUNCTION: RBR 0x004022f0
void* Object004022f0::sub_004022f0(unsigned int flags) {
    void* block = field_04;
    field_00 = (unsigned int)gVtable0076f3e8;
    if (block != 0) {
        HeapFree(GetProcessHeap(), 0, block);
    }
    if ((flags & 1) != 0) {
        sub_006efdd0(this);
    }
    return this;
}

// FUNCTION: RBR 0x00402970
const char* Object00402970::sub_00402970() const {
    if (field_24 >= 0x10) {
        return field_10_pointer;
    }
    return field_10_buffer;
}

// FUNCTION: RBR 0x00402980
void* Object00402980::sub_00402980(unsigned int flags) {
    sub_00402a00();
    if (flags & 1) {
        sub_006efdd0(this);
    }
    return this;
}
