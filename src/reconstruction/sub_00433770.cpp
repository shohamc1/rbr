// File-interface write callback (slot 2 of the callback table built in
// sub_0041f4f0 at 0x0041f511). cdecl(self, stream, buffer, size): forwards to
// fwrite(buffer, 1, size, stream) and discards the result. The first parameter
// is the table's user_data slot, which the stdio implementation ignores.
// Compiled with MSVC 7.1 (/O2 /Zl).
extern "C" {
__declspec(dllimport) unsigned int __cdecl fwrite(const void *buffer,
                                                  unsigned int size,
                                                  unsigned int count,
                                                  void *stream);
}

extern "C" void __cdecl sub_00433770(void *self, void *stream,
                                     const void *buffer, unsigned int size) {
    fwrite(buffer, 1, size, stream);
}
