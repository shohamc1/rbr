// sub_0046e270: __stdcall, one dword parameter,
// dword return (RET 4). Stored at dispatch-table slot 0x68 (EXE 0x00710779c)
// of the screen/coordinate class; it ignores the ECX this pointer.
//
// Indexes the container object at 0x00874618 through sub_004ba120 (thiscall
// helper: ECX base, index parameter, returns base + 0x14c + index * 0x1c,
// a 0x1c-stride record array) and returns the record's +0x18 field.
// sub_0046e290 is the +0x14 twin.

class Record004ba120 {
public:
    unsigned char unknown_00[0x14];
    unsigned int field_14;
    unsigned int field_18;
};

class Object004ba120 {
public:
    Record004ba120* sub_004ba120(unsigned int index);
};

typedef char Record004ba120_size[(sizeof(Record004ba120) == 0x1c) ? 1 : -1];

extern "C" Object004ba120* gObject00874618; // EXE .data 0x00874618

// FUNCTION: RBR 0x0046e270
extern "C" unsigned int __stdcall sub_0046e270(unsigned int index) {
    return gObject00874618->sub_004ba120(index)->field_18;
}
