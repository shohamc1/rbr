// sub_0050a8f0: MSVC 7.1 (/O2 /Zl).
//
// Scalar deleting destructor, vtable slot 0 of the vtable at 0x0071e03c (same
// family as sub_00402980). Calls the real destructor sub_00503210 (547-byte
// body, thiscall by register; its stub symbol is the cdecl symbol
// _sub_00503210, so it is called with no arguments while ECX still holds
// this), then frees the object through the CRT operator-delete thunk
// sub_006efdd0 (jmp [__imp_??3@YAXPAX@Z]) when bit 0 of the flags argument is
// set. thiscall, one stack argument, returns this in EAX.

extern "C" void __cdecl sub_00503210();             // 0x00503210, real destructor (ECX = this)
extern "C" void __cdecl sub_006efdd0(void* memory); // 0x006efdd0, operator delete thunk

struct Object0050a8f0 {
    void* sub_0050a8f0(unsigned int flags);
};

// sub_0050a910: MSVC 7.1 (/O2 /Zl).
//
// Scalar deleting destructor, vtable slot 0 of the vtable at 0x0071e094 (the
// destructor sub_00503550 stores that vtable address back into the object).
// Calls the real destructor sub_00503550 (61-byte body, thiscall member of the
// same class), then frees the object through the CRT operator-delete thunk
// sub_006efdd0 (jmp [__imp_??3@YAXPAX@Z]) when bit 0 of the flags argument is
// set. Same shape as the matched sub_00402980. thiscall, one stack argument,
// returns this in EAX.

extern "C" void __cdecl sub_006efdd0(void* memory); // 0x006efdd0, operator delete thunk

struct Object0050a910 {
    void* sub_0050a910(unsigned int flags);
    void sub_00503550(); // 0x00503550, real destructor (thiscall)
};

// sub_0050ab80: MSVC 7.1 (/O2 /Zl).
//
// thiscall member (two stack arguments, returns a float in st(0), ret 8).
// The receiver holds two array bases: field_04 (records of at least 0x108+4
// bytes with byte stride 0x40 per index) and field_08 (records of at least
// 0x88+4 bytes with byte stride 0x38 per index). The method clamps
// (field_04[index].0x108 + addend) to a byte, stores it at
// field_08[index].0x88 (raising a zero result to 1), and returns the scaled
// difference of sub_00504230(value, record) and sub_00504230(value - 1,
// record) through the float constant at 0x0071e09c (20.3718319, part of the
// constant pool that follows the 0x0071e094 vtable).

struct Object0050ab80 {
    unsigned char unknown_00[4];
    void* field_04;
    void* field_08;
    float sub_0050ab80(int index, unsigned int addend);
    float sub_00504230(unsigned int value, unsigned int* record); // 0x00504230
};

// FUNCTION: RBR 0x0050a8f0
void* Object0050a8f0::sub_0050a8f0(unsigned int flags) {
    sub_00503210();
    if (flags & 1) {
        sub_006efdd0(this);
    }
    return this;
}

// FUNCTION: RBR 0x0050a910
void* Object0050a910::sub_0050a910(unsigned int flags) {
    sub_00503550();
    if (flags & 1) {
        sub_006efdd0(this);
    }
    return this;
}

// FUNCTION: RBR 0x0050ab80
float Object0050ab80::sub_0050ab80(int index, unsigned int addend) {
    unsigned int* record =
        (unsigned int*)((char*)field_08 + index * 0x38 + 0x88);
    unsigned int value =
        (*(unsigned int*)((char*)field_04 + index * 0x40 + 0x108) + addend) & 0xff;
    *record = value;
    if (value == 0) {
        value = 1;
    }
    float first = sub_00504230(value, record);
    float second = sub_00504230(value - 1, record);
    return (first - second) * (64.0f / 3.14159265f);
}
