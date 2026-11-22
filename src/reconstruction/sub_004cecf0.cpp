// sub_004cecf0: EXE 0x004cecf0, 17 bytes. thiscall member with two dword
// stack arguments (RET 8).
//
// Two-field setter: stores the first argument at +0x10 and the second at
// +0x14 (arguments loaded into EAX and EDX before the stores). Byte-for-byte
// twin of sub_004cecd0; the class identity of the two receivers is not
// established, so they stay separate objects.
class Object004cecf0 {
public:
    void sub_004cecf0(unsigned int first, unsigned int second);
};

void Object004cecf0::sub_004cecf0(unsigned int first, unsigned int second) {
    *(unsigned int *)((char *)this + 0x10) = first;
    *(unsigned int *)((char *)this + 0x14) = second;
}
