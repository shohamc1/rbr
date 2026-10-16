// Game-launch teardown called by game main (sub_00471110) after the intro
// launcher (sub_00470ea0). When the success flag at 0x007ca1ec is set, waits
// forever on the intro child process handle at 0x007ca1dc, closes both child
// handles (0x007ca1dc process, 0x007ca1e0 thread) from that PROCESS_INFORMATION
// record, and clears the flag. __cdecl, no parameters, bare RET.
extern "C" {
__declspec(dllimport) unsigned long __stdcall WaitForSingleObject(
    void *handle, unsigned long milliseconds);
__declspec(dllimport) int __stdcall CloseHandle(void *handle);
}

extern "C" void __cdecl sub_00470e60() {
    if (*(unsigned char *)0x007ca1ec) {
        WaitForSingleObject(*(void **)0x007ca1dc, (unsigned long)-1);
        CloseHandle(*(void **)0x007ca1dc);
        CloseHandle(*(void **)0x007ca1e0);
        *(unsigned char *)0x007ca1ec = 0;
    }
}
