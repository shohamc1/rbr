// sub_00402b50: EXE 0x00402b50, 96 bytes. MSVC 7.1 (/O2 /Zl).
//
// Streams a float triple to std::ostream as "(x, y, z)":
//   stream << "(" << x << ", " << y << ", " << z << ")";
// __cdecl with the ostream reference and three by-value floats; result
// discarded, bare RET.
//
// MSVCP71 imports (see msvcp71.h for the mangling rules: char_traits must be
// a struct): the free template operator<<<char_traits<char> > for const char*
// through IAT 0x0070c344, folded into one load and four CALL ESI sites, and
// the thiscall member basic_ostream::operator<<(float) through IAT 0x0070c274.
// The strings are the EXE .rdata literals, addressed by their pinned
// addresses ("(" 0x0076f478, ", " 0x0076f474, ")" 0x0076f480) so the object
// defines no local data.

namespace std {
    template<class Element> struct char_traits;

    template<class Element, class Traits>
    class basic_ostream {
    public:
        __declspec(dllimport) basic_ostream& operator<<(float value);
    };

    template<class Traits>
    __declspec(dllimport)
    basic_ostream<char, Traits>& __cdecl operator<<(basic_ostream<char, Traits>& stream,
                                                    const char* text);
}

// NONMATCHING (7 instructions, 89 of 96 bytes): under /O2 the compiler folds
// the basic_ostream::operator<<(float) import into EDI (push/pop EDI,
// "mov edi,[0x70c274]" once, three "call edi"), while the reference keeps
// three "call dword ptr [0x70c274]" sites; the mid-run stream reload shifts
// from [esp+0x10] to [esp+0x14] accordingly. The free operator<< import folds
// into ESI in both. Tried without improvement: split statements (loses the
// argument push-run), explicit operator<< call syntax, a local function
// pointer initialized from the dllimport address, __declspec(dllimport) on
// the class (disables the push-run and the ESI fold; adds an EBP frame),
// #pragma optimize("g"/"t"), and /G5, /G7, /Oi.
extern "C" void __cdecl sub_00402b50(std::basic_ostream<char, std::char_traits<char> >& stream,
                                     float x, float y, float z) {
    stream << (const char*)0x0076f478 << x
           << (const char*)0x0076f474 << y
           << (const char*)0x0076f474 << z
           << (const char*)0x0076f480;
}
