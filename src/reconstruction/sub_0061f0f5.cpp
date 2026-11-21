// sub_0061f0f5: EXE 0x0061f0f5, 109 bytes. MSVC 7.1 (/O2 /Zl) with the
// file-local size pragma (PUSH -8/POP EDI constant materialization, INC
// pair in the memset size, memory state reloads).
//
// Accumulation member: for each of the field_68 16-byte elements, adds the
// four floats of element i+1 of the array at +0x5c into element i of the
// argument array, then zeroes the first (field_68 + 2) elements of the
// +0x5c array with the memset intrinsic (REP STOSD/STOSB). The source
// element address is rebuilt each iteration from the reloaded +0x5c base
// and the destination walker. __thiscall with one stack word (RET 4).
// Callers: 33 sites in 0x0061f162-0x00625eab.

#pragma optimize("s", on)

extern "C" void* __cdecl memset(void*, int, unsigned int);
#pragma intrinsic(memset)

class Object0061f0f5 {
public:
    unsigned char unknown_00[0x5c];
    void* field_5c;
    unsigned char unknown_60[8];
    unsigned int field_68;

    void sub_0061f0f5(void* elements);
};

struct Elements0061f0f5 {
    float element[4];
};

void Object0061f0f5::sub_0061f0f5(void* elements) {
    unsigned int i;
    for (i = 0; i < field_68; i++) {
        Elements0061f0f5* source = &((Elements0061f0f5*)field_5c)[i + 1];
        Elements0061f0f5* destination = (Elements0061f0f5*)elements + i;
        destination->element[0] += source->element[0];
        destination->element[1] += source->element[1];
        destination->element[2] += source->element[2];
        destination->element[3] += source->element[3];
    }
    memset(field_5c, 0, (field_68 + 2) * 16);
}
