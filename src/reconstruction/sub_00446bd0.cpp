// GamePlatform0044a3c0 command-line setter, called by game main (sub_00471110)
// with the process command line. Assigns the argument to the member string at
// offset 0xd64. The reference tail-calls the MSVCP71 import slot 0x0070c2f8,
// std::basic_string<char>::operator=(const basic_string&), with ECX advanced
// to the member and the caller's argument left on the stack.
#include "msvcp71.h"

struct GamePlatform0044a3c0 {
    unsigned char field_0000[0xd64];
    std::string commandLine;
    void sub_00446bd0(const std::string& arguments);
};
typedef char platform_command_line_assertion[
    ((unsigned int)&((GamePlatform0044a3c0*)0)->commandLine) == 0xd64 ? 1 : -1];

void GamePlatform0044a3c0::sub_00446bd0(const std::string& arguments) {
    commandLine = arguments;
}
