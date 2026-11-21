// sub_005ebdb0: EXE 0x005ebdb0, 16 bytes. Constructor (ECX-in/EAX-out).
//
// Zero-initializer for the shared fixed-array container header used by the
// 0x005ebdc0-0x005ebe50 accessors in this region: an allocation cursor at +0,
// a second counter at +4, a byte flag at +8 (with padding), and the element
// base pointer at +0xc. Clears all four fields.

class Object005ebdb0 {
public:
    unsigned int field_00;
    unsigned int field_04;
    unsigned char field_08;
    unsigned char unknown_09[3];
    void *field_0c;

    Object005ebdb0();
};

Object005ebdb0::Object005ebdb0() {
    field_00 = 0;
    field_04 = 0;
    field_08 = 0;
    field_0c = 0;
}
