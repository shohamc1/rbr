// sub_005ebdc0: EXE 0x005ebdc0, 19 bytes. __thiscall member, no parameters.
//
// Fixed-array container accessor: returns the address of the next 0x50-byte
// element from the base pointer at +0xc and post-increments the allocation
// cursor at +0. Shares the container header shape with sub_005ebdb0 (cursor
// +0, counter +4, flag byte +8, base +0xc).

struct Element005ebdc0 { unsigned char data[0x50]; };

class Object005ebdc0 {
public:
    unsigned int field_00;
    unsigned int field_04;
    unsigned char field_08;
    unsigned char unknown_09[3];
    Element005ebdc0 *field_0c;

    void *sub_005ebdc0();
};

void *Object005ebdc0::sub_005ebdc0() {
    Element005ebdc0 *element = &field_0c[field_00];
    field_00 = field_00 + 1;
    return element;
}
