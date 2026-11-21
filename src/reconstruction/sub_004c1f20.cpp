// sub_004c1f20: EXE 0x004c1f20, 18 bytes. __thiscall member (ECX = this)
// with one dword stack argument (RET 4), pointer result in EAX.
//
// Byte-identical twin of sub_004c1f00 kept as a separate object per the
// one-function rule: same array accessor shape for the 0x004c1xxx container
// family, record array base at +4, 0x50-byte records (LEA x5 plus SHL 4).
// No direct callers in Ghidra; the original was a second instantiation or
// table slot of the same accessor.
struct Record004c1f20 { unsigned char bytes[0x50]; };

class Object004c1f20 {
public:
    unsigned char unknown_00[4];
    Record004c1f20 *field_04;

    Record004c1f20 *sub_004c1f20(unsigned int index);
};

Record004c1f20 *Object004c1f20::sub_004c1f20(unsigned int index) {
    return field_04 + index;
}
