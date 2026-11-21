// sub_004ba120: EXE 0x004ba120, 17 bytes. MSVC 7.1 (/O2 /Zl).
//
// thiscall member with one cdecl stack argument (RET 4), dword return.
// Indexes the container object at 0x00874618 (see the matched callers
// sub_0046e270/sub_0046e290): returns this + 0x14c + index * 0x1c, the
// 0x1c-stride record array that starts at offset 0x14c of that container.
// The class and record names match the matched callers' declarations so one
// symbol resolves both call sites.

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

Record004ba120* Object004ba120::sub_004ba120(unsigned int index) {
    return (Record004ba120*)((char*)this + 0x14c) + index;
}
