// sub_00545b50: EXE 0x00545b50, 11 bytes. __cdecl, no arguments, bare RET.
// Clears the dword flag at EXE 0x0163edbc (sub_005475f0 sets it to 1).
// Registered as a callback from sub_00547d60 at 0x00548250. Compiled with
// MSVC 7.1 (/O2 /Zl).

extern "C" void __cdecl sub_00545b50() {
    *(unsigned int *)0x0163edbc = 0;
}
