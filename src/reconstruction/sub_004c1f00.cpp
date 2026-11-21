// sub_004c1f00: EXE 0x004c1f00, 18 bytes. __thiscall member (ECX = this)
// with one dword stack argument (RET 4), pointer result in EAX.
//
// Array accessor of the 0x004c1xxx container family: the record array base
// sits at +4 and each record is 0x50 bytes (MSVC strength-reduces
// index * 0x50 to LEA x5 plus SHL 4). No direct callers in Ghidra;
// consistent with dispatch through a table such as 0x007137b4.
struct Record004c1f00 { unsigned char bytes[0x50]; };

class Object004c1f00 {
public:
    unsigned char unknown_00[4];
    Record004c1f00 *field_04;

    Record004c1f00 *sub_004c1f00(unsigned int index);
};

Record004c1f00 *Object004c1f00::sub_004c1f00(unsigned int index) {
    return field_04 + index;
}
