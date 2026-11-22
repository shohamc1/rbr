// sub_00402c80: EXE 0x00402c80, 69 bytes. MSVC 7.1 (/O2 /Zl).
//
// String sink of the log/stream class referenced from the record at
// 0x0076f450 (same receiver layout as sub_00402c20): flag bit 1 at +0x40
// mirrors to OutputDebugStringA, flag bit 0 writes through the FILE* at
// +0x3c with fprintf("%s") followed by fflush. Returns its second argument
// (the byte count) unchanged, like a streambuf write-back. thiscall, two
// stack arguments, RET 8.
//
// Imports: KERNEL32 OutputDebugStringA (IAT 0x0070c1c0, stdcall), MSVCR71
// fprintf (0x0070c59c) and fflush (0x0070c5d0), cdecl; the single
// "add esp,0x10" cleans the three fprintf pushes and the fflush push.

extern "C" {
__declspec(dllimport) void __stdcall OutputDebugStringA(const char* text);
__declspec(dllimport) int __cdecl fprintf(void* stream, const char* format, ...);
__declspec(dllimport) int __cdecl fflush(void* stream);
}

struct Object00402c80 {
    unsigned char field_00[0x3c];
    void* field_3c;            // FILE* sink
    unsigned char field_40;    // mirror flags: bit 0 file, bit 1 debug string
    int sub_00402c80(const char* text, int length);
};

int Object00402c80::sub_00402c80(const char* text, int length) {
    if (field_40 & 2) {
        OutputDebugStringA(text);
    }
    if ((field_40 & 1) && field_3c != 0) {
        fprintf(field_3c, "%s", text);
        fflush(field_3c);
    }
    return length;
}
