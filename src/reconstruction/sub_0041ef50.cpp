// sub_0041ef50: EXE 0x0041ef50, 18 bytes. MSVC 7.1 (/O2 /Zl).
//
// thiscall cursor advance that chains: field_0 += index * 0x1c on the record
// pointer stored at +0 and returns this in EAX (one stack argument, RET 4).
// Returning this is what copies ECX to EAX at entry and moves the index to
// ECX. sub_0041ef10 reads the same field; sub_0041eec0/sub_0041eee0 address a
// sibling container through its +4 field. Names are hypotheses.

struct Record0041ef50 {
    char bytes[0x1c];
};
typedef char Record0041ef50SizeAssert[(sizeof(Record0041ef50) == 0x1c) ? 1 : -1];

class Object0041ef50 {
public:
    Object0041ef50 *sub_0041ef50(int index);

private:
    Record0041ef50 *field_0;   // +0: record cursor
    unsigned char field_4[4];  // +4: unknown
};

Object0041ef50 *Object0041ef50::sub_0041ef50(int index) {
    field_0 += index;
    return this;
}
