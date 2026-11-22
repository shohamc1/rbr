// File-interface open callback (slot 0 of the callback table built in
// sub_0041f4f0 at 0x0041f501). The interface is a C-style cdecl callback
// table: (open, read, write, tell, seek, close, error, user_data); the first
// parameter of every callback is the table's user_data slot, which the stdio
// implementation ignores. This one maps open() mode bits to CRT mode strings
// and calls fopen: bit0 set with bits0-1 == 1 selects "rb" (0x007767f4),
// bit2 "r+b" (0x007767f0), bit3 "wb" (0x007767ec). The mode strings are
// referenced by their .rdata addresses; the caller at 0x0041f552 passes
// (self, path, 5). Compiled with MSVC 7.1 (/O2 /Zl).
extern "C" {
__declspec(dllimport) void *__cdecl fopen(const char *path, const char *mode);
}

extern "C" void *__cdecl sub_00433700(void *self, const char *path, int flags) {
    void *result = 0;
    const char *mode = 0;
    if ((flags & 3) == 1) {
        mode = "rb";
    } else if (flags & 4) {
        mode = "r+b";
    } else if (flags & 8) {
        mode = "wb";
    }
    if (path != 0 && mode != 0) {
        result = fopen(path, mode);
    }
    return result;
}
