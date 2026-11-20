// GamePlatform0044a3c0 command-line setter, called by game main (sub_00471110)
// with the process command line. Assigns the argument to the member string at
// offset 0xd64. The reference tail-calls the MSVCP71 import slot 0x0070c2f8,
// std::basic_string<char>::operator=(const basic_string&), with ECX advanced
// to the member and the caller's argument left on the stack.
namespace std {
    template<class Element> struct char_traits;
    template<class Element> class allocator;
    template<class Element, class Traits, class Allocator> class basic_string;
    // The EXE imports this member from MSVCP71.dll; char_traits must be a
    // struct and allocator a class to keep the mangled name identical.
    template<> class __declspec(dllimport) basic_string<char, char_traits<char>, allocator<char> > {
        char buffer[0x10];
        unsigned int length;
        unsigned int capacity;
        unsigned int allocatorField;
    public:
        basic_string& operator=(const basic_string& other);
    };
}

typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > PlatformString;
typedef char platform_string_size_assertion[(sizeof(PlatformString) == 0x1c) ? 1 : -1];

struct GamePlatform0044a3c0 {
    unsigned char field_0000[0xd64];
    PlatformString commandLine;
    void sub_00446bd0(const PlatformString& arguments);
};
typedef char platform_command_line_assertion[
    ((unsigned int)&((GamePlatform0044a3c0*)0)->commandLine) == 0xd64 ? 1 : -1];

void GamePlatform0044a3c0::sub_00446bd0(const PlatformString& arguments) {
    commandLine = arguments;
}
