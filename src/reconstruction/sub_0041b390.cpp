// sub_0041b390: EXE 0x0041b390, 36 bytes. MSVC 7.1 (/O2 /Zl).
//
// Narrow (char) twin of sub_0041b310: stream-buffer guard constructor that
// stores the stream pointer at +0 and, when the stream's streambuf exists,
// calls the MSVCP71 thiscall import std::basic_streambuf<char,
// char_traits<char>>::_Lock (IAT 0x0070c2e4). Returns this in EAX, RET 4
// (constructor convention). The streambuf pointer is read through the stream
// subobject's virtual-base table: vbtable[1] (+4) is the byte offset of the
// virtual basic_ios base, whose +0x28 field is the streambuf pointer.

namespace std {
    template<class Element> struct char_traits;

    template<class Element, class Traits>
    class basic_streambuf {
    public:
        __declspec(dllimport) void _Lock();
    };
}

typedef std::basic_streambuf<char, std::char_traits<char> > CharStreambuf;

class Object0041b390 {
public:
    Object0041b390(void *stream);

private:
    void *stream;  // +0: guarded stream subobject (first word: vbptr)
};

Object0041b390::Object0041b390(void *streamValue) : stream(streamValue) {
    char *sub = (char *)streamValue;
    int basicIosOffset = *(int *)(*(char **)sub + 4);  // vbtable[1]
    CharStreambuf *buffer = *(CharStreambuf **)(sub + basicIosOffset + 0x28);
    if (buffer != 0)
        buffer->_Lock();
}
