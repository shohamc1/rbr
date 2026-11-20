// Game-launch helpers called in sequence by game main (sub_00471110): the
// startup program launcher (sub_00470f00), the intro launcher (sub_00470ea0)
// and the intro teardown (sub_00470e60). sub_00470ea0 and sub_00470e60 share
// the PROCESS_INFORMATION record at 0x007ca1dc and the flag at 0x007ca1ec.
// Win32 declarations are in win32.h. Compiled with MSVC 7.1 (/O2 /Zl).
#include "win32.h"

extern "C" void *__cdecl memset(void *, int, unsigned int);
#pragma intrinsic(memset)

// Game-launch teardown called by game main (sub_00471110) after the intro
// launcher (sub_00470ea0). When the success flag at 0x007ca1ec is set, waits
// forever on the intro child process handle at 0x007ca1dc, closes both child
// handles (0x007ca1dc process, 0x007ca1e0 thread) from that PROCESS_INFORMATION
// record, and clears the flag. __cdecl, no parameters, bare RET.
extern "C" void __cdecl sub_00470e60() {
    if (*(unsigned char *)0x007ca1ec) {
        WaitForSingleObject(*(void **)0x007ca1dc, (unsigned long)-1);
        CloseHandle(*(void **)0x007ca1dc);
        CloseHandle(*(void **)0x007ca1e0);
        *(unsigned char *)0x007ca1ec = 0;
    }
}

// Game-launch helper called by game main (sub_00471110) before the CPUID probe.
// Launches "Intro.exe" (literal .rdata command-line pointer 0x007108d0) with an
// empty STARTUPINFOA (cb = 0x44) on the stack and a PROCESS_INFORMATION record
// at the literal globals 0x007ca1dc-0x007ca1e8; success sets the byte flag at
// 0x007ca1ec. __cdecl, no parameters, return value unused by the caller.
//
// The checker resolves the CreateProcessA IAT import; data addresses are literal.
//
// NONMATCHING: the instruction set is exact (89 bytes, same encodings), but
// the /O2 scheduler weaves the five stores into the argument-push run while
// the reference keeps the three PROCESS_INFORMATION zero stores contiguous
// right after the REP STOSD and both remaining stores after the push run.
// Tried without improvement: volatile qualifiers, chained assignments, loop
// and aggregate inits, memset-on-literal expansions, struct value-init,
// statement reorders, and #pragma optimize("s") (EBP frame; wrong).

extern "C" void __cdecl sub_00470ea0() {
    StartupInfoA si;
    memset(&si, 0, sizeof(si));
    *(void**)0x007ca1dc = 0;
    *(void**)0x007ca1e0 = 0;
    *(unsigned long*)0x007ca1e4 = 0;
    *(unsigned long*)0x007ca1e8 = 0;
    si.cb = sizeof(si);
    if (CreateProcessA(0, (char*)0x007108d0, 0, 0, 0, 0, 0, 0, &si,
                       (void*)0x007ca1dc)) {
        *(unsigned char*)0x007ca1ec = 1;
    }
}

// Launches "RBR Startup.exe" (literal command line at 0x007108dc) and closes
// both child handles. Called by game main sub_00471110.
extern "C" bool __cdecl sub_00470f00() {
    StartupInfoA startup_info;
    ProcessInformation process_info;
    memset(&startup_info, 0, sizeof(startup_info));
    startup_info.cb = sizeof(startup_info);
    memset(&process_info, 0, sizeof(process_info));
    if (!CreateProcessA(0, (void *)0x7108dc, 0, 0, 0, 0, 0, 0, &startup_info,
                        &process_info)) {
        return false;
    }
    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);
    return true;
}
