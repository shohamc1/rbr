// sub_0041b310: EXE 0x0041b310, 36 bytes. MSVC 7.1 (/O2 /Zl).
//
// Stream-buffer guard constructor: stores the stream pointer at +0 and, when
// the stream's streambuf exists, calls the MSVCP71 thiscall import
// std::basic_streambuf<unsigned short, char_traits<unsigned short>>::_Lock
// (IAT 0x0070c25c). Returns this in EAX, RET 4 (constructor convention). The
// streambuf pointer is read through the stream subobject's virtual-base
// table: vbtable[1] (+4) is the byte offset of the virtual basic_ios base,
// whose +0x28 field is the streambuf pointer. sub_0041b360 is byte-identical
// (a second instantiation); sub_0041b390 is the narrow (char) twin;
// sub_0041b250/sub_0041b340 are the matching _Unlock paths.

namespace std {
    template<class Element> struct char_traits;

    template<class Element, class Traits>
    class basic_streambuf {
    public:
        __declspec(dllimport) void _Lock();
    };
}

typedef std::basic_streambuf<unsigned short, std::char_traits<unsigned short> >
    WideStreambuf;

class Object0041b310 {
public:
    Object0041b310(void *stream);

private:
    void *stream;  // +0: guarded stream subobject (first word: vbptr)
};

Object0041b310::Object0041b310(void *streamValue) : stream(streamValue) {
    char *sub = (char *)streamValue;
    int basicIosOffset = *(int *)(*(char **)sub + 4);  // vbtable[1]
    WideStreambuf *buffer = *(WideStreambuf **)(sub + basicIosOffset + 0x28);
    if (buffer != 0)
        buffer->_Lock();
}
