// sub_0043a2c0: EXE 0x0043a2c0, 35 bytes. Compiled with MSVC 7.1 (/O2 /Zl).
//
// Backward move for 4-byte elements: memmove(dst_end - count, first,
// count * 4) with count = last - first. The element count is computed once
// (SUB, SAR 2), scaled once (SHL 2), and the byte size is reused for both
// the destination and the size argument. The memmove import is called
// through its IAT slot (0x0070c510) as a dllimport. cdecl.
extern "C" __declspec(dllimport) void* __cdecl memmove(void*, const void*,
                                                       unsigned int);

extern "C" void __cdecl sub_0043a2c0(const unsigned int* first,
                                     const unsigned int* last,
                                     unsigned int* dst_end) {
    int count = last - first;
    memmove(dst_end - count, first, count * 4);
}
