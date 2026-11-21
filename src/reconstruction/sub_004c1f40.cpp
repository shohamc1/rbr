// sub_004c1f40: EXE 0x004c1f40, 12 bytes. __thiscall member (ECX = this)
// with one pointer stack argument (RET 4).
//
// Out-parameter getter of the 0x004c1xxx container family: stores the
// record-array base at +4 through the caller-supplied pointer. No direct
// callers in Ghidra; consistent with dispatch through a table such as
// 0x007137b4.
class Object004c1f40 {
public:
    unsigned char unknown_00[4];
    void *field_04;

    void **sub_004c1f40(void **slot);
};

void **Object004c1f40::sub_004c1f40(void **slot) {
    *slot = field_04;
    return slot;
}
