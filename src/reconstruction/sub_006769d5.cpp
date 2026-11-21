// sub_006769d5: EXE 0x006769d5, 12 bytes. __thiscall member, no parameters.
//
// Returns the element count for the byte size stored at +0x4: the size is
// rounded up to a multiple of four and given two extra units
// ((size + 3) / 4 + 2). Callers (0x0065117e, 0x0065143e, 0x006b73dd,
// 0x006b7a23) pass this in ECX; the result is a plain unsigned value.
//
// The reference adds two with INC EAX twice: the file-local size pragma
// selects that encoding over ADD EAX,2.

#pragma optimize("s", on)

class Object006769d5 {
public:
    unsigned char unknown_00[0x4];
    unsigned int field_04;

    unsigned int sub_006769d5();
};

unsigned int Object006769d5::sub_006769d5() {
    return (field_04 + 3) / 4 + 2;
}
