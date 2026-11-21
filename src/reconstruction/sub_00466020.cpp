// sub_00466020: EXE 0x00466020, 39 bytes. Copy constructor, __thiscall with
// one stack argument (the source), returns this in EAX.
//
// Copies the source in declaration order: the word at +0x00 through DX, then
// the 12-byte POD tail at +0x04 as the MSVC 12-byte assignment shape
// (source cursor ADD ECX,4; ESI/ECX loads; EDX = this+4 stores). The two
// bytes at +0x02 are padding and are skipped. No incoming references are
// known; the record layout is body evidence.

struct Tail00466020 {
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
};

class Object00466020 {
public:
    unsigned short field_00;
    unsigned char unknown_02[2];
    Tail00466020 field_04;

    Object00466020(const Object00466020& other);
};

Object00466020::Object00466020(const Object00466020& other)
    : field_00(other.field_00), field_04(other.field_04) {}
