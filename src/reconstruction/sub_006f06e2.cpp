// Nonmatching CRT initializer at EXE 0x006f06e2, through RET at 0x006f0725.
// The linker sentinels both resolve to EXE 0x00788080. Separate declarations
// retain the loop. Literal equal addresses would remove it during compilation.
// The reference uses an SEH frame with its filter at 0x006f070f and handler
// at 0x006f0713. Scope data and funclets still need linker-aware checking.
#pragma optimize("s", on)

typedef void (__cdecl *Initializer)(void);
extern "C" Initializer const gRtcInitializersBegin[];
extern "C" Initializer const gRtcInitializersEnd[];

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
