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

struct StartupInfoA00470ea0 {
    unsigned long cb;
    char* reserved;
    char* desktop;
    char* title;
    unsigned long x, y, width, height;
    unsigned long count_x, count_y, fill_attribute, flags;
    unsigned short show_window, cb_reserved2;
    unsigned char* reserved2;
    void* std_input;
    void* std_output;
    void* std_error;
};

extern "C" void* __cdecl memset(void*, int, unsigned int);
#pragma intrinsic(memset)

extern "C" {
__declspec(dllimport) int __stdcall CreateProcessA(
    const void* application_name,
    void* command_line,
    const void* process_attributes,
    const void* thread_attributes,
    int inherit_handles,
    unsigned long creation_flags,
    const void* environment,
    const void* current_directory,
    void* startup_info,
    void* process_information);
}

extern "C" void __cdecl sub_00470ea0() {
    StartupInfoA00470ea0 si;
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
