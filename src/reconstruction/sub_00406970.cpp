// sub_00406970: EXE 0x00406970, 21 bytes. MSVC 7.1 (/O2 /Zl).
//
// Indexed setter: the receiver holds an array of 0x74-byte records at +0x10
// (the Collection00416390/004163c0 record size; see analysis/notes.md), and
// this writes the last dword of the selected record:
//   field_10[index].field_70 = value;
// thiscall, two stack arguments, RET 8. No callers are in the manifest yet.

struct Object00406970Record {
    unsigned char body[0x70];
    unsigned int field_70;
};

struct Object00406970 {
    unsigned char field_00[0x10];
    Object00406970Record* field_10;
    void sub_00406970(unsigned int index, unsigned int value);
};

void Object00406970::sub_00406970(unsigned int index, unsigned int value) {
    field_10[index].field_70 = value;
}
