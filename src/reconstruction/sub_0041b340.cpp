// sub_0041b340: EXE 0x0041b340, 24 bytes. MSVC 7.1 (/O2 /Zl).
//
// Byte-identical twin of sub_0041b250 (a second instantiation of the same
// stream-buffer guard): when the stream stored at +0 has a streambuf,
// tail-jumps to the MSVCP71 thiscall import std::basic_streambuf<unsigned
// short, char_traits<unsigned short>>::_Unlock (the same IAT 0x0070c2bc). The
// streambuf pointer is read through the stream subobject's virtual-base
// table: vbtable[1] (+4) is the byte offset of the virtual basic_ios base,
// whose +0x28 field is the streambuf pointer. Called from 0x00704733 and
// 0x00704bbb.

namespace std {
    template<class Element> struct char_traits;

    template<class Element, class Traits>
    class basic_streambuf {
    public:
        __declspec(dllimport) void _Unlock();
    };
}

typedef std::basic_streambuf<unsigned short, std::char_traits<unsigned short> >
    WideStreambuf;

class Object0041b340 {
public:
    void sub_0041b340();

private:
    void *stream;  // +0: guarded stream subobject (first word: vbptr)
};

void Object0041b340::sub_0041b340() {
    char *sub = (char *)stream;
    int basicIosOffset = *(int *)(*(char **)sub + 4);  // vbtable[1]
    WideStreambuf *buffer = *(WideStreambuf **)(sub + basicIosOffset + 0x28);
    if (buffer != 0)
        buffer->_Unlock();
}
