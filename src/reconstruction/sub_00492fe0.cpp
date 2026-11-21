// sub_00492fe0: EXE 0x00492fe0, 86 bytes. Buffered one-byte writer of the
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
