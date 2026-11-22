// sub_0041eec0: MSVC 7.1 (/O2 /Zl).
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

// sub_0041eee0: MSVC 7.1 (/O2 /Zl).
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

// sub_0041ef10: MSVC 7.1 (/O2 /Zl).
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

// sub_0041ef50: MSVC 7.1 (/O2 /Zl).
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

// FUNCTION: RBR 0x0041eec0
Record0041eec0 *Object0041eec0::sub_0041eec0(int index) {
    return field_4 + index;
}

// FUNCTION: RBR 0x0041eee0
Record0041eee0 **Object0041eee0::sub_0041eee0(Record0041eee0 **out) {
    *out = (Record0041eee0 *)field_4;
    return out;
}

// FUNCTION: RBR 0x0041ef10
Record0041ef10 **Object0041ef10::sub_0041ef10(Record0041ef10 **out, int index) {
    *out = field_0 + index;
    return out;
}

// FUNCTION: RBR 0x0041ef50
Object0041ef50 *Object0041ef50::sub_0041ef50(int index) {
    field_0 += index;
    return this;
}
