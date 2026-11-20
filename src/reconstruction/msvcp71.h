// MSVCP71.dll declarations shared by the MSVC 7.1 sources. The EXE reaches
// every member below through the import address table, so each is declared
// __declspec(dllimport) and never defined here.
//
// Mangling is load-bearing. char_traits must be a struct (mangles as U) and
// allocator a class (mangles as V), or the import names differ from the EXE
// slots, for example 0x0070c2d8 (destructor) and 0x0070c2f8 (operator=).
// Only the members that a source uses cost bytes; unused declarations emit
// nothing. The size and member layout (0x1c bytes) come from sub_0040fab0,
// sub_00446bd0 and sub_00471110, which all reproduce the same import names.
#pragma once

namespace std {
    template<class Element> struct char_traits;
    template<class Element> class allocator;
    template<class Element, class Traits, class Allocator> class basic_string;

    template<> class __declspec(dllimport) basic_string<char, char_traits<char>, allocator<char> > {
        char buffer[0x10];
        unsigned int length;
        unsigned int capacity;
        unsigned int allocatorField;
    public:
        basic_string(const char* text);
        ~basic_string();
        basic_string& operator=(const basic_string& other);
    };

    typedef basic_string<char, char_traits<char>, allocator<char> > string;

    // Import __imp_?_Nomemory@std@@YAXXZ; operator new (sub_006efa32) calls it.
    __declspec(dllimport) void __cdecl _Nomemory();
}

typedef char std_string_size_assertion[(sizeof(std::string) == 0x1c) ? 1 : -1];
