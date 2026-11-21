// sub_004a5ee0: EXE 0x004a5ee0, 17 bytes. MSVC 7.1 (/O2 /Zl).
//
// thiscall two-word setter: copies the first stack argument to +0x10 and the
// second to +0x14 in order, RET 8. No incoming references in the Ghidra
// index; the argument kinds (words versus pointers) are not evidenced, so
// they are modelled as unsigned ints.

class Object004a5ee0 {
public:
    unsigned char field_00[0x10];
    unsigned int field_10;
    unsigned int field_14;
    void sub_004a5ee0(unsigned int first, unsigned int second);
};

void Object004a5ee0::sub_004a5ee0(unsigned int first, unsigned int second)
{
    field_10 = first;
    field_14 = second;
}
