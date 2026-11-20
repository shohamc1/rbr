// Narrows a NUL-terminated 16-bit string into an 8-bit buffer. A character with
// any bit set in 0xff00 becomes 0xff. Returns the destination, which the
// reference keeps in EAX.

typedef unsigned short u16;

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
