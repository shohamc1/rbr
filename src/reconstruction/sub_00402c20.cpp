// sub_00402c20: EXE 0x00402c20, 85 bytes. MSVC 7.1 (/O2 /Zl).
//
// Character sink of the log/stream class referenced from the record at
// 0x0076f438: flag bit 1 at +0x40 mirrors to OutputDebugStringA, flag bit 0
// writes through the FILE* at +0x3c with fprintf("%c") followed by fflush.
// The -1 input is the "no character" sentinel and returns unchanged. The
// two-byte string for OutputDebugStringA is built in the incoming argument
// slot. thiscall, one stack argument, returns the character in EAX.
//
// Imports: KERNEL32 OutputDebugStringA (IAT 0x0070c1c0, stdcall), MSVCR71
// fprintf (0x0070c59c) and fflush (0x0070c5d0), cdecl. The single
// "add esp,0x10" cleans the three fprintf pushes and the fflush push.

extern "C" {
__declspec(dllimport) void __stdcall OutputDebugStringA(const char* text);
__declspec(dllimport) int __cdecl fprintf(void* stream, const char* format, ...);
__declspec(dllimport) int __cdecl fflush(void* stream);
}

struct Object00402c20 {
    unsigned char field_00[0x3c];
    void* field_3c;            // FILE* sink
    unsigned char field_40;    // mirror flags: bit 0 file, bit 1 debug string
    int sub_00402c20(int character);
};

int Object00402c20::sub_00402c20(int character) {
    if (character != -1) {
        if (field_40 & 2) {
            char text[2];
            text[0] = (char)character;
            text[1] = 0;
            OutputDebugStringA(text);
        }
        if ((field_40 & 1) && field_3c != 0) {
            fprintf(field_3c, "%c", character);
            fflush(field_3c);
        }
    }
    return character;
}
