// Exit-handler registration from the statically linked MSVC runtime.
// `_atexit` (sub_006efecf) calls `__onexit` (sub_006efea9), which keeps the
// handler table pointers at 0x016e3510 and 0x016e350c. Both need the
// file-local size optimization: under plain /O2 MSVC 13.10.3077 cleans cdecl
// arguments with `add esp,N`, while the reference uses POP-based epilogues
// (see analysis/notes.md). Compiled with MSVC 7.1 (/O2 /Zl).
#pragma optimize("s", on)

extern "C" unsigned int __cdecl sub_006f06ac(unsigned int handler, void* begin, void* end);

// CRT __onexitbegin/__onexitend handler table bounds.
extern "C" void* gOnexitBegin; // EXE .data 0x016e3510
extern "C" void* gOnexitEnd;   // EXE .data 0x016e350c

extern "C" __declspec(dllimport) unsigned int __cdecl _onexit(unsigned int handler);

extern "C" unsigned int __cdecl sub_006efea9(unsigned int handler) {
    if (gOnexitBegin == (void*)-1) {
        return _onexit(handler);
    }
    return sub_006f06ac(handler, &gOnexitBegin, &gOnexitEnd);
}

extern "C" int __cdecl _atexit(unsigned int handler) {
    return sub_006efea9(handler) == 0 ? -1 : 0;
}
