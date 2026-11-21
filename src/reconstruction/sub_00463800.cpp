// sub_00463800: EXE 0x00463800, 16 bytes. Default constructor, __thiscall,
// no parameters, returns this in EAX.
//
// Zeroes the first four byte-sized members of the object. Evidence: the
// adjacent constructor sub_00463810 (EXE 0x00463810) allocates four bytes
// with the operator-new helper sub_006efa32 and inlines this body as
// "xor ecx,ecx" plus the same four byte stores before storing the block in
// the wrapper's field_0; sub_00463840 deletes that block. This class is
// therefore the four-byte pointee of that wrapper.

class Object00463800 {
public:
    unsigned char field_00;
    unsigned char field_01;
    unsigned char field_02;
    unsigned char field_03;

    Object00463800();
};

Object00463800::Object00463800() {
    field_00 = 0;
    field_01 = 0;
    field_02 = 0;
    field_03 = 0;
}
