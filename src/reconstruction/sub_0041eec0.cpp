// sub_0041eec0: EXE 0x0041eec0, 15 bytes. MSVC 7.1 (/O2 /Zl).
//
// thiscall accessor on an array-holder object: returns (in EAX) the address
// of the index-th 0x1c-byte record, computed as field_4 + index * 0x1c from
// the record-array pointer stored at +4. RET 4 (one stack argument). Names
// are hypotheses from this function alone; the record size matches the
// 0x1c-byte MSVCP-style record seen elsewhere (see analysis/notes.md).

struct Record0041eec0 {
    char bytes[0x1c];
};
typedef char Record0041eec0SizeAssert[(sizeof(Record0041eec0) == 0x1c) ? 1 : -1];

class Object0041eec0 {
public:
    Record0041eec0 *sub_0041eec0(int index);

private:
    unsigned char field_0[4];      // +0: unknown
    Record0041eec0 *field_4;       // +4: record array base
};

Record0041eec0 *Object0041eec0::sub_0041eec0(int index) {
    return field_4 + index;
}
