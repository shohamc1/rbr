// sub_0041b250: EXE 0x0041b250, 24 bytes. MSVC 7.1 (/O2 /Zl).
//
// Stream-buffer guard member over the stream pointer stored at +0: when the
// stream's streambuf exists, tail-jumps to the MSVCP71 thiscall import
// std::basic_streambuf<unsigned short, char_traits<unsigned short>>::_Unlock
// (IAT 0x0070c2bc). The streambuf pointer is read through the stream
// subobject's virtual-base table: vbtable[1] (+4) is the byte offset of the
// virtual basic_ios base, whose +0x28 field is the streambuf pointer.
// sub_0041b340 is byte-identical (a second instantiation of the same guard);
// sub_0041b2f0 is the narrow (char) twin. Called from 0x00704713.

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

class Object0041b250 {
public:
    void sub_0041b250();

private:
    void *stream;  // +0: guarded stream subobject (first word: vbptr)
};

void Object0041b250::sub_0041b250() {
    char *sub = (char *)stream;
    int basicIosOffset = *(int *)(*(char **)sub + 4);  // vbtable[1]
    WideStreambuf *buffer = *(WideStreambuf **)(sub + basicIosOffset + 0x28);
    if (buffer != 0)
        buffer->_Unlock();
}
