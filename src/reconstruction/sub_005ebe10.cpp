// sub_005ebe10: EXE 0x005ebe10, 17 bytes. __thiscall member, no parameters.
//
// Fixed-array container accessor for 0xb4-byte elements: returns the address
// of the next element from the base pointer at +0xc and post-increments the
// allocation cursor at +0. Same container header shape as sub_005ebdb0.

struct Element005ebe10 { unsigned char data[0xb4]; };

class Object005ebe10 {
public:
    unsigned int field_00;
    unsigned int field_04;
    unsigned char field_08;
    unsigned char unknown_09[3];
    Element005ebe10 *field_0c;

    void *sub_005ebe10();
};

void *Object005ebe10::sub_005ebe10() {
    Element005ebe10 *element = &field_0c[field_00];
    field_00 = field_00 + 1;
    return element;
}
