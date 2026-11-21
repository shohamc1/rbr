// sub_005538a0: EXE 0x005538a0, 13 bytes. __thiscall member with one dword
// stack argument (RET 4). Stores the argument to the dword at +0x468. No
// direct references found in the EXE; the owner class is unproved, keep the
// local class. Compiled with MSVC 7.1 (/O2 /Zl).

class Object005538a0 {
public:
    unsigned char unknown_00[0x468];
    unsigned int field_468;
    void sub_005538a0(unsigned int value);
};

typedef char Object005538a0_offset[((unsigned int)&(((Object005538a0*)0)->field_468) == 0x468) ? 1 : -1];

void Object005538a0::sub_005538a0(unsigned int value) { field_468 = value; }
