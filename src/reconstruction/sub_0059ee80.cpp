// Widens a NUL-terminated 8-bit string into a 16-bit buffer. Returns the
// destination.

typedef unsigned short u16;

extern "C" u16* __cdecl sub_0059ee80(const char* source, u16* destination) {
    u16* out = destination;
    do {
        *out++ = static_cast<unsigned char>(*source);
    } while (*source++ != 0);
    return destination;
}
