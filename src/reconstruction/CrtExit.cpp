#include "msvcp71.h"


extern "C" int __cdecl _callnewh(unsigned int size);
extern "C" void* __cdecl malloc(unsigned int size);

// Exit-handler registration from the statically linked MSVC runtime.
// `_atexit` (sub_006efecf) calls `__onexit` (sub_006efea9), which keeps the
// handler table pointers at 0x016e3510 and 0x016e350c. Both need the
// file-local size optimization: under plain /O2 MSVC 13.10.3077 cleans cdecl
// arguments with `add esp,N`, while the reference uses POP-based epilogues
// (see analysis/notes.md). Compiled with MSVC 7.1 (/O2 /Zl).

extern "C" unsigned int __cdecl sub_006f06ac(unsigned int handler, void* begin, void* end);

// CRT __onexitbegin/__onexitend handler table bounds.
extern "C" void* gOnexitBegin; // EXE .data 0x016e3510
extern "C" void* gOnexitEnd;   // EXE .data 0x016e350c

extern "C" __declspec(dllimport) unsigned int __cdecl _onexit(unsigned int handler);

// Nonmatching CRT initializer at EXE 0x006f06e2, through RET at 0x006f0725.
// The linker sentinels both resolve to EXE 0x00788080. Separate declarations
// retain the loop. Literal equal addresses would remove it during compilation.
// The reference uses an SEH frame with its filter at 0x006f070f and handler
// at 0x006f0713. Scope data and funclets still need linker-aware checking.

typedef void (__cdecl *Initializer)(void);
extern "C" Initializer const gRtcInitializersBegin[];
extern "C" Initializer const gRtcInitializersEnd[];

extern "C" unsigned int __cdecl _controlfp(unsigned int value, unsigned int mask);

#pragma optimize("s", on)
// FUNCTION: RBR 0x006efa32
extern "C" void* __cdecl sub_006efa32(unsigned int size) {
    void* block;
    while ((block = malloc(size)) == 0) {
        if (_callnewh(size) == 0) {
            std::_Nomemory();
        }
    }
    return block;
}
#pragma optimize("", on)

#pragma optimize("s", on)
// FUNCTION: RBR 0x006efea9
extern "C" unsigned int __cdecl sub_006efea9(unsigned int handler) {
    if (gOnexitBegin == (void*)-1) {
        return _onexit(handler);
    }
    return sub_006f06ac(handler, &gOnexitBegin, &gOnexitEnd);
}
#pragma optimize("", on)

#pragma optimize("s", on)

// FUNCTION: RBR 0x006efecf
extern "C" int __cdecl _atexit(unsigned int handler) {
    return sub_006efea9(handler) == 0 ? -1 : 0;
}
#pragma optimize("", on)

#pragma optimize("s", on)
// NONMATCHING
// FUNCTION: RBR 0x006f06e2
extern "C" void __cdecl __RTC_Initialize() {
    for (Initializer const* cursor = gRtcInitializersBegin;
         cursor < gRtcInitializersEnd; ++cursor) {
        __try {
            if (*cursor != 0) {
                (*cursor)();
            }
        } __except (1) {
        }
    }
}
#pragma optimize("", on)

#pragma optimize("s", on)
// FUNCTION: RBR 0x006f0770
extern "C" void __cdecl __setdefaultprecision() {
    _controlfp(0x10000, 0x30000);
}
#pragma optimize("", on)

// FUNCTION: RBR 0x006f078b
extern "C" int __cdecl sub_006f078b() {
    return 0;
}
