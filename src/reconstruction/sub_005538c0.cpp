// sub_005538c0: EXE 0x005538c0, 10 bytes. __thiscall member with one dword
// stack argument (RET 4) that the body never reads: a notification-style
// callback that just sets the dword at +0x38 to 1. No direct references found
// in the EXE; the owner class is unproved, keep the local class. Compiled with
// MSVC 7.1 (/O2 /Zl).

class Object005538c0 {
public:
    unsigned char unknown_00[0x38];
    unsigned int field_38;
    void sub_005538c0(unsigned int unused);
};

typedef char Object005538c0_offset[((unsigned int)&(((Object005538c0*)0)->field_38) == 0x38) ? 1 : -1];

void Object005538c0::sub_005538c0(unsigned int unused) { field_38 = 1; }
