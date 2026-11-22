// sub_005ebdb0: Constructor (ECX-in/EAX-out).
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

// sub_005ebdc0: __thiscall member, no parameters.
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

// sub_005ebdf0: __thiscall member, one int
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

// sub_005ebe10: __thiscall member, no parameters.
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

// sub_005ebe40: __thiscall member, one int
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

// sub_005ebe50: __thiscall member, one int
// parameter (RET 4).
//
// Fixed-array container accessor for 0x28-byte elements: returns the address
// of element `index` from the base pointer at +0xc without touching the
// cursor at +0. Same container header shape as sub_005ebdb0/sub_005ebdc0.

struct Element005ebe50 { unsigned char data[0x28]; };

class Object005ebe50 {
public:
    unsigned int field_00;
    unsigned int field_04;
    unsigned char field_08;
    unsigned char unknown_09[3];
    Element005ebe50 *field_0c;

    void *sub_005ebe50(int index);
};

// FUNCTION: RBR 0x005ebdb0
Object005ebdb0::Object005ebdb0() {
    field_00 = 0;
    field_04 = 0;
    field_08 = 0;
    field_0c = 0;
}

// FUNCTION: RBR 0x005ebdc0
void *Object005ebdc0::sub_005ebdc0() {
    Element005ebdc0 *element = &field_0c[field_00];
    field_00 = field_00 + 1;
    return element;
}

// FUNCTION: RBR 0x005ebdf0
void *Object005ebdf0::sub_005ebdf0(int index) {
    return &field_0c[index];
}

// FUNCTION: RBR 0x005ebe10
void *Object005ebe10::sub_005ebe10() {
    Element005ebe10 *element = &field_0c[field_00];
    field_00 = field_00 + 1;
    return element;
}

// FUNCTION: RBR 0x005ebe40
void *Object005ebe40::sub_005ebe40(int index) {
    return &field_0c[index];
}

// FUNCTION: RBR 0x005ebe50
void *Object005ebe50::sub_005ebe50(int index) {
    return &field_0c[index];
}
