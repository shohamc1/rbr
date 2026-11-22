#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
struct Child004659f0 { unsigned char unknown_00[0x370]; unsigned int field_370; };
struct Object004659f0 { unsigned char unknown_00[0x70]; Child004659f0 *field_70; void sub_004659f0(unsigned int value); };
typedef char offset_check[offsetof(Object004659f0, field_70) == 0x70 ? 1 : -1];
typedef char offset_check[offsetof(Child004659f0, field_370) == 0x370 ? 1 : -1];

// sub_00466020: Copy constructor, __thiscall with
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

// FUNCTION: RBR 0x004659f0
void Object004659f0::sub_004659f0(unsigned int value) { field_70->field_370 = value; }

// FUNCTION: RBR 0x00466020
Object00466020::Object00466020(const Object00466020& other)
    : field_00(other.field_00), field_04(other.field_04) {}
