// sub_00545c50: EXE 0x00545c50, 29 bytes. __cdecl with one dword argument
// (registered as a callback from sub_00547d60 at 0x00547d44). The receiver
// object at EXE 0x0163f0a8 is loaded into ECX before the branch: when the
// argument is zero, virtual slot 1 of that object receives (0x6d, 0) via
// CALL [EAX+4]; otherwise the function tail-jumps into sub_0056f440
// (manifest source:null), which reads the same ECX as this (PUSH ESI;
// MOV ESI,ECX prologue, fields +8/+0xc/+0x14/+0x18/+0x2c, bare RET) — a
// thiscall callee, so the call goes through a same-shape member to keep
// ECX live across both paths (queue: symbol ?sub_0056f440@Object0056f440@@QAEXXZ).
// Compiled with MSVC 7.1 (/O2 /Zl).

class Object0163f0a8 {
public:
    virtual void sub_007264e4_slot0();
    virtual void slot_04(unsigned int a, unsigned int b);
};

class Object0056f440 {
public:
    void sub_0056f440();
};

extern "C" void __cdecl sub_00545c50(unsigned int argument) {
    Object0163f0a8 *receiver = *(Object0163f0a8 **)0x0163f0a8;
    if (argument == 0) {
        receiver->slot_04(0x6d, 0);
    } else {
        ((Object0056f440 *)(void *)receiver)->sub_0056f440();
    }
}
