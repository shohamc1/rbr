// sub_0056f260: EXE 0x0056f260, 10 bytes. MSVC 7.1 (/O2 /Zl).
//
// thiscall setter with one cdecl stack argument (RET 4) of the quickrace
// name-format object at EXE 0x007b184c: stores the argument at +0x10.
// The matched callback sub_005526d0 passes "qr%02d_out.arp" (0x00726f2c)
// here; sub_0056f250 is the +0x0c twin. Both callees are members of the
// same class in the callers' declarations.

class Object0056f250 {
public:
    unsigned char unknown_00[0xc];
    const char* field_0c;
    const char* field_10;
    void sub_0056f250(const char* name);
    void sub_0056f260(const char* name);
};

void Object0056f250::sub_0056f260(const char* name) {
    field_10 = name;
}
