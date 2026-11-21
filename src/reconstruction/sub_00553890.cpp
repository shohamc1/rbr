// sub_00553890: EXE 0x00553890, 8 bytes. __thiscall member, no stack
// arguments, bare RET. Increments the dword at +0x4c and returns the new
// value: the EAX roundtrip (load, one-byte INC EAX, store) only appears when
// the result is used, so the reference reads as a pre-increment with its value
// returned. No direct references found in the EXE; the owner class of +0x4c is
// unproved, keep the local class.

class Object00553890 {
public:
    unsigned char unknown_00[0x4c];
    unsigned int field_4c;
    unsigned int sub_00553890();
};

typedef char Object00553890_offset[((unsigned int)&(((Object00553890*)0)->field_4c) == 0x4c) ? 1 : -1];

unsigned int Object00553890::sub_00553890() { return ++field_4c; }
