// File-interface error callback (slot 6 of the callback table built in
// sub_0041f4f0 at 0x0041f531). cdecl(self, stream): returns
// stream->field_c & 0x20 without calling the CRT. The layout matches the
// MSVCRT FILE structure, whose _flag field sits at offset 0xc; bit 0x20 is
// _IOERR, so this is an ferror() specialization reading the flag word
// directly. The first parameter is the table's user_data slot, which the
// stdio implementation ignores. Compiled with MSVC 7.1 (/O2 /Zl).
struct StreamFile {
    unsigned char field_0[0xc];  // _ptr, _cnt, _base
    unsigned int field_c;        // _flag
};

extern "C" unsigned int __cdecl sub_004337f0(void *self, StreamFile *stream) {
    return stream->field_c & 0x20;
}
