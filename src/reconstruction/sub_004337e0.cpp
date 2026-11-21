// File-interface close callback (slot 5 of the callback table built in
// sub_0041f4f0 at 0x0041f529). cdecl(self, stream): returns fclose(stream)
// unchanged. The first parameter is the table's user_data slot, which the
// stdio implementation ignores. Compiled with MSVC 7.1 (/O2 /Zl).
extern "C" {
__declspec(dllimport) int __cdecl fclose(void *stream);
}

extern "C" int __cdecl sub_004337e0(void *self, void *stream) {
    return fclose(stream);
}
