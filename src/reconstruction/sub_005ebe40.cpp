// sub_005ebe40: EXE 0x005ebe40, 15 bytes. __thiscall member, one int
// parameter (RET 4).
//
// Fixed-array container accessor for 0x78-byte elements: returns the address
// of element `index` from the base pointer at +0xc without touching the
// cursor at +0. Same container header shape as sub_005ebdb0/sub_005ebdc0.

struct Element005ebe40 { unsigned char data[0x78]; };

class Object005ebe40 {
public:
    unsigned int field_00;
    unsigned int field_04;
    unsigned char field_08;
    unsigned char unknown_09[3];
    Element005ebe40 *field_0c;

    void *sub_005ebe40(int index);
};

void *Object005ebe40::sub_005ebe40(int index) {
    return &field_0c[index];
}
