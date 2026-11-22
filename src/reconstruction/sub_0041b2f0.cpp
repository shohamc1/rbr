// sub_0041b2f0: MSVC 7.1 (/O2 /Zl).
//
// Narrow (char) twin of sub_0041b250: stream-buffer guard member over the
// stream pointer stored at +0; when the stream's streambuf exists, tail-jumps
// to the MSVCP71 thiscall import std::basic_streambuf<char,
// char_traits<char>>::_Unlock (IAT 0x0070c2ec). The streambuf pointer is read
// through the stream subobject's virtual-base table: vbtable[1] (+4) is the
// byte offset of the virtual basic_ios base, whose +0x28 field is the
// streambuf pointer. Called from 0x007046f3.

namespace std {
    template<class Element> struct char_traits;

    template<class Element, class Traits>
    class basic_streambuf {
    public:
        __declspec(dllimport) void _Unlock();
    };
}

typedef std::basic_streambuf<char, std::char_traits<char> > CharStreambuf;

class Object0041b2f0 {
public:
    void sub_0041b2f0();

private:
    void *stream;  // +0: guarded stream subobject (first word: vbptr)
};

// FUNCTION: RBR 0x0041b2f0
void Object0041b2f0::sub_0041b2f0() {
    char *sub = (char *)stream;
    int basicIosOffset = *(int *)(*(char **)sub + 4);  // vbtable[1]
    CharStreambuf *buffer = *(CharStreambuf **)(sub + basicIosOffset + 0x28);
    if (buffer != 0)
        buffer->_Unlock();
}
