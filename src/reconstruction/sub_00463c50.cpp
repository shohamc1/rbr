// sub_00463c50: EXE 0x00463c50, 9 bytes. __thiscall getter, no parameters.
//
// Returns field_04 + 0x12c: a pointer 0x12c bytes past the pointer stored
// at offset 4, i.e. the address of a sub-record inside the pointee. The
// sibling getter at 0x00463c60 is identical with +0x258. Pointee identity
// and the sub-record layout are unknown.

class Object00463c50 {
public:
    unsigned char unknown_00[4];
    unsigned char* field_04;

    unsigned char* sub_00463c50();
};

unsigned char* Object00463c50::sub_00463c50() {
    return field_04 + 0x12c;
}
