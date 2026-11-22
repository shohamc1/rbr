// sub_00442340: EXE 0x00442340, 34 bytes. __cdecl forwarder of the intrusive
// list/node subsystem to the record-copy helper sub_0043fe10 (EXE 0x0043fe10,
// cdecl, reads (dst, count, src); parameters beyond the third are declared
// but unused at every call site). This function passes its four stack
// arguments straight through as (dst, count, src, fourth) and an
// indeterminate 4-byte local as the fifth argument: the PUSH ECX prologue
// slot is MSVC 7.1's indeterminate-local idiom, read back and pushed as the
// helper's fifth argument. Plain RET confirms __cdecl with four arguments;
// ECX is never read (unlike sibling sub_00443360, which forwards its
// thiscall ECX there and cleans with RET 0xc). No callers reference this
// body anywhere in the EXE. MSVC 7.1 (/O2 /Zl).

extern "C" void __cdecl sub_0043fe10(void* dst, int count, void* src,
                                     void* fourth, void* fifth);

extern "C" void __cdecl sub_00442340(void* dst, int count, void* src,
                                     void* fourth) {
    void* fifth;
    sub_0043fe10(dst, count, src, fourth, fifth);
}
