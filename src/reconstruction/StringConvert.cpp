// Conversions between 8-bit and 16-bit NUL-terminated strings. Both are
// cdecl and return the destination in EAX. The crash filter sub_00470f70
// calls both in one function to convert the computer name, the date text and
// the dump file name. Compiled with MSVC 7.1 (/O2 /Zl).

typedef unsigned short u16;

// Narrows a NUL-terminated 16-bit string into an 8-bit buffer. A character with
// any bit set in 0xff00 becomes 0xff. Returns the destination, which the
// reference keeps in EAX.
extern "C" char* __cdecl sub_0059e9d0(const u16* source, char* destination) {
    char* out = destination;
    u16 c = *source;
    while (c != 0) {
        if ((c & 0xff00) > 0) {
            *out = static_cast<char>(0xff);
        } else {
            *out = static_cast<char>(*source);
        }
        ++source;
        ++out;
        c = *source;
    }
    *out = 0;
    return destination;
}

// Widens a NUL-terminated 8-bit string into a 16-bit buffer. Returns the
// destination.
extern "C" u16* __cdecl sub_0059ee80(const char* source, u16* destination) {
    u16* out = destination;
    do {
        *out++ = static_cast<unsigned char>(*source);
    } while (*source++ != 0);
    return destination;
}
