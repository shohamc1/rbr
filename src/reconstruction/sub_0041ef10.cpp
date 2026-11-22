// sub_0041ef10: EXE 0x0041ef10, 20 bytes. MSVC 7.1 (/O2 /Zl).
//
// thiscall out-getter over the record cursor stored at +0 that chains:
// writes field_0 + index * 0x1c into the caller's output slot and returns
// that slot in EAX (two stack arguments, RET 8). The return value places the
// out pointer in EAX while the address computation uses EDX/EAX. Unlike
// sub_0041eec0, the base here is the +0 field; sub_0041ef50 advances the same
// field. Names are hypotheses.

struct Record0041ef10 {
    char bytes[0x1c];
};
typedef char Record0041ef10SizeAssert[(sizeof(Record0041ef10) == 0x1c) ? 1 : -1];

class Object0041ef10 {
public:
    Record0041ef10 **sub_0041ef10(Record0041ef10 **out, int index);

private:
    Record0041ef10 *field_0;   // +0: record cursor
    unsigned char field_4[4];  // +4: unknown
};

Record0041ef10 **Object0041ef10::sub_0041ef10(Record0041ef10 **out, int index) {
    *out = field_0 + index;
    return out;
}
