// sub_0061ea99: MSVC 7.1 (/O2 /Zl) with the
// file-local size pragma (POP ECX cleanup).
//
// Scalar deleting destructor, slot 0 of the vtable at EXE 0x00734090 (the
// store target of the base teardown sub_0061d989). Calls the base teardown
// member sub_0061d989 on this while ECX still holds this, then frees this
// through sub_006efdd0 when bit 0 of the flags word is set, and returns
// this. __thiscall with one stack word (RET 4).


extern "C" void __cdecl sub_006efdd0(void* memory);

class Object0061d989 {
public:
    void sub_0061d989();
};

class Object0061ea99 {
public:
    void* sub_0061ea99(unsigned int flags);
};

// sub_0061f0f5: MSVC 7.1 (/O2 /Zl) with the
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

#pragma optimize("s", on)
// FUNCTION: RBR 0x0061ea99
void* Object0061ea99::sub_0061ea99(unsigned int flags) {
    ((Object0061d989*)this)->sub_0061d989();
    if (flags & 1)
        sub_006efdd0(this);
    return this;
}
#pragma optimize("", on)

#pragma optimize("s", on)
// FUNCTION: RBR 0x0061f0f5
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
#pragma optimize("", on)
