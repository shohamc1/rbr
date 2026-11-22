// sub_00423a80: EXE 0x00423a80, 28 bytes. MSVC 7.1 (/O2 /Zl).
//
// Register-argument forwarder: __stdcall with four stack words (mangled
// _sub_00423a80@16) whose address is stored into the callback record built
// around 0x0040ce80. It forwards (arg2, arg1, global, arg3) to the /GS
// classifier at sub_00423d50 as a __fastcall call: arg2 lands in ECX (it
// becomes this via `sub ecx,0x110` there), arg1 rides in EDX, and the two
// stack words are the dword at the BSS global 0x016e3050 (read by absolute
// address; neighbor of the FILE* global 0x016e3044) and arg3. arg4 is
// accepted and cleaned by RET 0x10 but never forwarded.

extern "C" void __fastcall sub_00423d50(void* self, void* arg_edx, void* global_value,
                                        void* arg3);

extern "C" void __stdcall sub_00423a80(void* arg1, void* arg2, void* arg3, void* arg4) {
    sub_00423d50(arg2, arg1, *(void**)0x016e3050, arg3);
}
