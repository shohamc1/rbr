// Runs the diagnostics handler chain selected by game main (RunDiagnose).
//
// The handler parameter reaches sub_0041c1b0 in EDI: the callee performs
// `call edi` throughout and never assigns EDI, and 0x0041c820 is its default
// handler, invoked indirectly. With no handler the reference opens
// "Diagnostics.txt" ("wt") through MSVCR71 fopen, keeps the FILE* in the
// global at 0x016e3044, runs the chain, then closes and clears that FILE*.
//
// MSVC 7.1 has no source form for an EDI register argument, so the callee is
// declared cdecl with a stack argument: the only decoration the checker's
// REL32 resolver accepts for the linker symbol. This keeps the handler
// live so the optimizer reserves ESI and EBX as in the reference; the draft
// stays nonmatching on the EDI handoff bytes (57/8B FE/5F) and on the
// caller-side stack cleanup the register form avoids.
typedef int __cdecl DiagnosticsHandler();

extern "C" {
__declspec(dllimport) void* __cdecl fopen(const char* filename, const char* mode);
__declspec(dllimport) int __cdecl fclose(void* stream);
int __cdecl sub_0041c1b0(DiagnosticsHandler* handler);
int __cdecl sub_0041c5f0();
int __cdecl sub_0041c820();  // default diagnostics handler
extern void* gDiagnosticsFile; // FILE*, EXE .data 0x016e3044
}

// NONMATCHING
// FUNCTION: RBR 0x0041c8a0
extern "C" int __cdecl sub_0041c8a0(DiagnosticsHandler* handler) {
    if (!handler) {
        gDiagnosticsFile = fopen("Diagnostics.txt", "wt");
        handler = sub_0041c820;
    }
    int result = sub_0041c1b0(handler);
    if (!result) {
        result = sub_0041c5f0();
    }
    void* file = gDiagnosticsFile;
    if (file) {
        fclose(file);
        gDiagnosticsFile = 0;
    }
    return result;
}
