// sub_004ceaa0: EXE 0x004ceaa0, 15 bytes. Constructor (ECX in, EAX = this).
//
// Saves this in ESI, runs the constructor of the embedded member at +0x50
// (EXE 0x004eb2c0: zeroes the three dwords at +0/+4/+8 of the subobject),
// and returns this in EAX. The 0x50 bytes before the member are not touched
// by this constructor.
class Object004eb2c0 {
public:
    Object004eb2c0();  // EXE 0x004eb2c0
};

class Object004ceaa0 {
public:
    Object004ceaa0();

private:
    unsigned char field_00[0x50];
    Object004eb2c0 field_50;
};

Object004ceaa0::Object004ceaa0() {}
