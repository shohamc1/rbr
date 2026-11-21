// File-interface tell callback (slot 3 of the callback table built in
// sub_0041f4f0 at 0x0041f519). cdecl(self, stream): returns ftell(stream)
// unchanged. The first parameter is the table's user_data slot, which the
// stdio implementation ignores. Compiled with MSVC 7.1 (/O2 /Zl).
extern "C" {
__declspec(dllimport) long __cdecl ftell(void *stream);
}

extern "C" long __cdecl sub_00433790(void *self, void *stream) {
    return ftell(stream);
}
