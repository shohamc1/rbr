// sub_00480920: EXE 0x00480920, 16 bytes. Default constructor, __thiscall,
// no parameters, returns this in EAX.
//
// Zeroes four dword members at +0x00/+0x04/+0x08/+0x0c from one zeroed
// register. The single incoming reference is a data word at 0x00480bf7
// (inside 0x00480af0's span), consistent with a dispatch or initializer
// table entry; the record layout is body evidence.

class Object00480920 {
public:
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;

    Object00480920();
};

Object00480920::Object00480920() {
    field_00 = 0;
    field_04 = 0;
    field_08 = 0;
    field_0c = 0;
}
