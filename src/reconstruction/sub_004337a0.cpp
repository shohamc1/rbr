// File-interface seek callback (slot 4 of the callback table built in
// sub_0041f4f0 at 0x0041f521). cdecl(self, stream, offset, origin): validates
// the origin (0, 1, or 2; anything else returns -1 without touching the
// stream), maps it to the identical CRT SEEK_* constant, calls
// fseek(stream, offset, whence), and always returns 0. The first parameter is
// the table's user_data slot, which the stdio implementation ignores. The
// caller at 0x0041f58b passes (self, stream, offset, 0). Compiled with
// MSVC 7.1 (/O2 /Zl).
extern "C" {
__declspec(dllimport) int __cdecl fseek(void *stream, long offset, int origin);
}

extern "C" int __cdecl sub_004337a0(void *self, void *stream, long offset,
                                    int origin) {
    int whence;
    switch (origin) {
    case 0:
        whence = 0;
        break;
    case 1:
        whence = 1;
        break;
    case 2:
        whence = 2;
        break;
    default:
        return -1;
    }
    fseek(stream, offset, whence);
    return 0;
}
