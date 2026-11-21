// sub_005ebdf0: EXE 0x005ebdf0, 18 bytes. __thiscall member, one int
// parameter (RET 4).
//
// Fixed-array container accessor for 0x50-byte elements: returns the address
// of element `index` from the base pointer at +0xc without touching the
// cursor at +0. Same container header shape as sub_005ebdb0/sub_005ebdc0.

struct Element005ebdf0 { unsigned char data[0x50]; };

class Object005ebdf0 {
public:
    unsigned int field_00;
    unsigned int field_04;
    unsigned char field_08;
    unsigned char unknown_09[3];
    Element005ebdf0 *field_0c;

    void *sub_005ebdf0(int index);
};

void *Object005ebdf0::sub_005ebdf0(int index) {
    return &field_0c[index];
}
