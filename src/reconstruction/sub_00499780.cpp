// sub_00499780: EXE 0x00499780, 10 bytes. __thiscall setter, one stack word.
// this->field_10 = value. Callers: sub_00451410 at 0x0045145f and
// sub_004514e0 at 0x00451505.

struct Object00499780 {
    unsigned char field_00[0x10];
    unsigned int field_10;
    void sub_00499780(unsigned int value);
};

void Object00499780::sub_00499780(unsigned int value)
{
    field_10 = value;
}
