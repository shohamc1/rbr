// File-interface read callback (slot 1 of the callback table built in
// sub_0041f4f0 at 0x0041f509). cdecl(self, stream, buffer, size): forwards to
// fread(buffer, 1, size, stream) and discards the result. The first parameter
// is the table's user_data slot, which the stdio implementation ignores.
// Compiled with MSVC 7.1 (/O2 /Zl).
extern "C" {
__declspec(dllimport) unsigned int __cdecl fread(void *buffer,
                                                 unsigned int size,
                                                 unsigned int count,
                                                 void *stream);
}

extern "C" void __cdecl sub_00433750(void *self, void *stream, void *buffer,
                                     unsigned int size) {
    fread(buffer, 1, size, stream);
}
