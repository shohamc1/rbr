// sub_004809d0: EXE 0x004809d0, 57 bytes. __thiscall member, no stack
// arguments, returns bool in AL (SETNZ after CMP EAX,-1).
//
// Reopen path of the file-wrapper record built by sub_00480930: closes the
// handle at +0x20 unless it holds the -1 sentinel (KERNEL32 CloseHandle,
// IAT 0x0070c18c), then opens the name at +0x24 with CreateFileA (IAT
// 0x0070c1b0) as GENERIC_READ(0x80000000), share 1, OPEN_EXISTING(3),
// flags 0x8000080, no template, stores the new handle at +0x20 and reports
// whether it differs from the -1 sentinel. No incoming references are
// known; the layout follows the constructor and sub_00480950.

#include "win32.h"

extern "C" __declspec(dllimport) void* __stdcall CreateFileA(
    const char* file_name, unsigned long desired_access,
    unsigned long share_mode, void* security_attributes,
    unsigned long creation_disposition, unsigned long flags_and_attributes,
    void* template_file);

class Object004809d0 {
public:
    unsigned char unknown_00[0x14];
    void* field_14;
    void* field_18;
    unsigned char unknown_1c[4];
    void* field_20;
    char field_24[260];

    bool sub_004809d0();
};

#pragma optimize("s", on)
bool Object004809d0::sub_004809d0() {
    if (field_20 != (void*)-1) {
        CloseHandle(field_20);
    }
    void* handle = CreateFileA(field_24, 0x80000000, 1, 0, 3, 0x8000080, 0);
    field_20 = handle;
    if (handle == (void*)-1) {
        return false;
    }
    return true;
}
