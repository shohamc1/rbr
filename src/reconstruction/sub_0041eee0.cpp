// sub_0041eee0: EXE 0x0041eee0, 12 bytes. MSVC 7.1 (/O2 /Zl).
//
// thiscall out-getter that chains: writes the record-array pointer stored at
// +4 into the caller's output slot and returns that slot in EAX (one stack
// argument, RET 4). The return value is what places the out pointer in EAX
// and the loaded value in ECX. Sibling of sub_0041eec0 (same +4 field,
// 0x1c-byte records); names are hypotheses.

struct Record0041eee0 {
    char bytes[0x1c];
};
typedef char Record0041eee0SizeAssert[(sizeof(Record0041eee0) == 0x1c) ? 1 : -1];

class Object0041eee0 {
public:
    Record0041eee0 **sub_0041eee0(Record0041eee0 **out);

private:
    unsigned char field_0[4];  // +0: unknown
    unsigned int field_4;      // +4: record array base
};

Record0041eee0 **Object0041eee0::sub_0041eee0(Record0041eee0 **out) {
    *out = (Record0041eee0 *)field_4;
    return out;
}
