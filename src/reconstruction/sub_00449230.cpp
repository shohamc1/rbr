// sub_00449230: EXE 0x00449230, 16 bytes. Element accessor for the container
// whose base pointer sits at +4: it returns base + index * 0x24, so the
// elements are 0x24-byte records. One thiscall stack argument, cleaned by the
// callee (RET 4). Compiled with MSVC 7.1 (/O2 /Zl); the reference scales the
// index by 9 with LEA and by 4 against the base in ECX.

class Object00449230 {
public:
    unsigned int field_00;
    void *field_04;   // element array base
    void *sub_00449230(unsigned int index);
};

void *Object00449230::sub_00449230(unsigned int index) {
    return (char *)field_04 + index * 0x24;
}
