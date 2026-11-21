// sub_0043a200: EXE 0x0043a200, 14 bytes. Compiled with MSVC 7.1 (/O2 /Zl).
//
// Deallocation helper for the 4-byte-element allocator family of
// sub_0043a290: forwards the first argument to the operator-delete thunk
// sub_006efdd0 (jmp [__imp_??3@YAXPAX@Z] at 0x006efdd0) and ignores the
// second argument, the element count. __stdcall with two words of
// arguments (RET 8); ECX is never read. The Ghidra packet drops the
// POP ECX/RET 8 tail; the raw EXE bytes carry the full 14-byte body.
extern "C" void __cdecl sub_006efdd0(void* block);   // operator delete thunk

extern "C" void __stdcall sub_0043a200(void* pointer, unsigned int) {
    sub_006efdd0(pointer);
}
