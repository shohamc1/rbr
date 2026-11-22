// sub_004022f0: EXE 0x004022f0, 54 bytes. MSVC 7.1 (/O2 /Zl).
//
// Scalar deleting destructor (MSVC ??_G shape) referenced from vtable
// 0x0076f3e8; byte-identical to sub_004021e0 except the restored vtable
// constant. Capture the heap block at +0x4, restore the vtable word, release
// the block through GetProcessHeap/HeapFree (IAT 0x0070c188/0x0070c184),
// free the object through the cdecl operator-delete thunk sub_006efdd0 when
// bit 0 of the flags argument is set, and return this in EAX. RET 4.

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

void* Object004022f0::sub_004022f0(unsigned int flags) {
    void* block = field_04;
    field_00 = 0x76f3e8;
    if (block != 0) {
        HeapFree(GetProcessHeap(), 0, block);
    }
    if ((flags & 1) != 0) {
        sub_006efdd0(this);
    }
    return this;
}
