// sub_00445e60: EXE 0x00445e60, 24 bytes. __thiscall member with one stack
// argument (RET 4), stored by sub_0044af60 into five consecutive dispatch
// tables at 0x0044c3ba/0x0044c3f5/0x0044c42f/0x0044c46a/0x0044c4a5, so it
// is reached virtually. Copies a 9-dword (0x24-byte) record from the
// caller's record into the embedded record at +0xd38: the struct
// assignment lowers to ESI/EDI/ECX=9 REP MOVSD. MSVC 7.1 (/O2 /Zl).

struct Record00445e60 {
    unsigned int words[9];
};

class Object00445e60 {
public:
    unsigned char unknown_00[0xd38];
    Record00445e60 field_d38;

    void sub_00445e60(const Record00445e60& record);
};

void Object00445e60::sub_00445e60(const Record00445e60& record) {
    field_d38 = record;
}
