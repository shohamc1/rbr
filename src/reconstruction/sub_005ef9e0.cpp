// sub_005ef9e0: EXE 0x005ef9e0, 22 bytes. __thiscall member, one pointer
// parameter (RET 4), returns the filled slot.
//
// Growable pointer array: appends `value` at the cursor +0x40 of the array
// base at +0x4c, post-increments the cursor, and returns the slot address.
// Callers sub_005ec600 (0x005ec8c5) and sub_005ecc00 (0x005ece1d) ignore the
// return value; the slot return is load-bearing for the register allocation
// (count in EDX, slot in EAX, value reloaded into ECX after the cursor store).

class Object005ef9e0 {
public:
    unsigned char unknown_00[0x40];
    unsigned int field_40;
    unsigned char unknown_44[8];
    void **field_4c;

    void **sub_005ef9e0(void *value);
};

void **Object005ef9e0::sub_005ef9e0(void *value) {
    void **slot = &field_4c[field_40];
    field_40 = field_40 + 1;
    *slot = value;
    return slot;
}
