// Object005c0db0 member: EXE 0x005c0db0, 39 bytes. __thiscall with one
// 4-byte stack argument (RET 4) passed by reference: loaded at [ESP+8] and
// dereferenced once. `this` is kept in ESI across the call.
//
// Same receiver class as the grow helper sub_005bf850 (0x005bf850, a stub in
// config/symbols.csv), which reads +0x0 data, +0x4 capacity, +0x8 size, and
// +0xc grow increment (all in dword elements): 0x005c2440, 0x005c2470, and
// 0x005c1840 pass it the same object. This member appends one element:
// grows once when size >= capacity (unsigned compare; JC skips the call),
// stores the dereferenced argument at data[size], then increments size.
// Ghidra found no incoming references; the address is likely taken or the
// caller is outside the analyzed set.

struct Object005c0db0 {
    unsigned int *field_00;   // element array
    unsigned int field_04;    // capacity in elements
    unsigned int field_08;    // size in elements
    unsigned int field_0c;    // grow increment (sub_005bf850 reads +0xc)

    void sub_005bf850();      // grow: reallocates data, capacity += field_0c
    void sub_005c0db0(const unsigned int &value);
};

// FUNCTION: RBR 0x005c0db0
void Object005c0db0::sub_005c0db0(const unsigned int &value) {
    if (field_08 >= field_04)
        sub_005bf850();
    field_00[field_08] = value;
    ++field_08;
}
