#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
// The entry returns this in EAX while it clears two words and one short.
struct Object00492020 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned short field_08;
    Object00492020();
};
typedef char offset_check[offsetof(Object00492020, field_08) == 8 ? 1 : -1];

// sub_00492fe0: Buffered one-byte writer of the
// logging neighborhood above sub_0048cc00. thiscall with one byte argument and
// RET 4, returning this in EAX. The record keeps a byte buffer base at +0, the
// buffered length at +4, the capacity at +8, and a polymorphic sink object at
// +0xc. With no sink the call is a no-op. When appending one more byte would
// exceed the capacity (length + 1 > capacity), the buffered run is flushed
// through sink virtual slot 1 (buffer, length), the length resets, and this
// byte is passed directly from the caller's stack slot (&value, 1); otherwise
// the byte lands at buffer[length] and the length increments (INC dword).
// Sink slot 0 is unidentified; slot 1 consumes (data, count).
// Compiled with MSVC 7.1 (/O2 /Zl).

class Sink00492fe0 {
public:
    virtual void Reserved0();
    virtual void Write(void *data, unsigned int count);
};

class Object00492fe0 {
public:
    void *field_00;          // byte buffer base
    unsigned int field_04;   // buffered length
    unsigned int field_08;   // capacity
    Sink00492fe0 *field_0c;  // sink object

    void *sub_00492fe0(unsigned char value);
};

#include "layout.h"
struct Object00493750 {
    unsigned char unknown_00[32];
    unsigned int field_20;
    unsigned int sub_00493750();
};
LAYOUT_OFFSET(Object00493750, field_20, 32);

#include "layout.h"
struct Object00493760 {
    unsigned char unknown_00[36];
    unsigned int field_24;
    unsigned int sub_00493760();
};
LAYOUT_OFFSET(Object00493760, field_24, 36);

#include "layout.h"
struct Object00493770 {
    unsigned char unknown_00[40];
    unsigned int field_28;
    unsigned int sub_00493770();
};
LAYOUT_OFFSET(Object00493770, field_28, 40);

// FUNCTION: RBR 0x00492020
Object00492020::Object00492020() { field_00 = 0; field_04 = 0; field_08 = 0; }

// NONMATCHING
// FUNCTION: RBR 0x00492fe0
void *Object00492fe0::sub_00492fe0(unsigned char value) {
    if (field_0c == 0) {
        return this;
    }
    unsigned int length = field_04;
    if (length + 1 > field_08) {
        if (field_00 != 0) {
            field_0c->Write(field_00, length);
        }
        field_04 = 0;
        field_0c->Write(&value, 1);
        return this;
    }
    ((unsigned char *)field_00)[length] = value;
    ++field_04;
    return this;
}

// FUNCTION: RBR 0x00493750
unsigned int Object00493750::sub_00493750() { return field_20; }

// FUNCTION: RBR 0x00493760
unsigned int Object00493760::sub_00493760() { return field_24; }

// FUNCTION: RBR 0x00493770
unsigned int Object00493770::sub_00493770() { return field_28; }
