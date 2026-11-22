// sub_004ce380: thiscall member returning float.
//
// Unsigned-to-float view of the dword field at +0x50 (the same field the
// dword getters at 0x004ce368/0x004ce378 read). MSVC 7.1 expands the
// conversion into FILD plus a 2^32 correction when the high bit is set; the
// __real@4f800000 bias resolves to the shared pool float at 0x00776a38.
#include "layout.h"

class Object004ce380 {
public:
    float sub_004ce380();

    char pad_00[0x50];
    unsigned int field_50;
};

LAYOUT_OFFSET(Object004ce380, field_50, 0x50);

// sub_004ce3d0: __stdcall with one dword argument.
//
// Returns the address of index-th element of the 0x18-stride record array
// based at 0x007186c0: the two-LEA chain scales the index by 24
// (LEA EAX,[EAX+EAX*2]; LEA EAX,[EAX*8+0x007186c0]). Callers in the
// 0x004ce4xx-0x004ce5xx container code index the same array; the value
// table scanned by sub_004ce3f0 sits just above it at 0x007a5820.
struct Record004ce3d0 {
    unsigned char bytes[0x18];
};

extern "C" Record004ce3d0 gRecords007186c0[]; // EXE .data 0x007186c0

// sub_004ce3f0: __stdcall with one dword argument.
//
// Membership test against the 80-entry dword table at 0x007a5820-0x007a5960
// (just above the 0x18-stride record array served by sub_004ce3d0): scans
// the table with EAX as the cursor, returns 0 on the first equal entry, and
// 1 when the cursor passes the end sentinel 0x007a5960. The compiler pads
// to the 16-byte loop head at 0x004ce400 with a 7-byte LEA ESP,[ESP+0].
// Called from the 0x004ce4xx container code with the dword from +0x50.
extern "C" unsigned int gTable007a5820[0x50]; // EXE .data 0x007a5820

// sub_004ce9a0: thiscall member with one pointer
// stack argument (RET 4).
//
// Copies a dword from the caller's record into this receiver's 0x2b0-stride
// element table: record->field_0 (loaded into EDX and scaled by IMUL) is the
// element index and record->field_0x98 (loaded into EAX) the value stored at
// this + 0xb0 + index*0x2b0. The only EXE address reference to this entry is
// the immediate at 0x004cf47a (inside a 0x004cf4xx body), not a vtable slot.
class Object004ce9a0 {
public:
    void sub_004ce9a0(void *record);
};

// sub_004ceaa0: Constructor (ECX in, EAX = this).
//
// Saves this in ESI, runs the constructor of the embedded member at +0x50
// (EXE 0x004eb2c0: zeroes the three dwords at +0/+4/+8 of the subobject),
// and returns this in EAX. The 0x50 bytes before the member are not touched
// by this constructor.
class Object004eb2c0 {
public:
    Object004eb2c0();  // EXE 0x004eb2c0
};

class Object004ceaa0 {
public:
    Object004ceaa0();

private:
    unsigned char field_00[0x50];
    Object004eb2c0 field_50;
};

// sub_004ceb90: thiscall member with two dword
// stack arguments (RET 8), returns a dword.
//
// Selector-indexed read from three parallel dword arrays in the receiver:
// selector 0 reads the table at +0xb00, selector 1 the table at +0xb08, and
// anything else the table at +0xb10, in each case at [this + table +
// index*4]. TEST/JZ then CMP/JNZ chain; each path reloads the index
// (EAX, then EDX, then EAX).
class Object004ceb90 {
public:
    unsigned int sub_004ceb90(unsigned int selector, unsigned int index);
};

// sub_004cecd0: thiscall member with two dword
// stack arguments (RET 8).
//
// Two-field setter: stores the first argument at +0x10 and the second at
// +0x14 (arguments loaded into EAX and EDX before the stores). Byte-for-byte
// twin of sub_004cecf0; the class identity of the two receivers is not
// established, so they stay separate objects.
class Object004cecd0 {
public:
    void sub_004cecd0(unsigned int first, unsigned int second);
};

// sub_004cecf0: thiscall member with two dword
// stack arguments (RET 8).
//
// Two-field setter: stores the first argument at +0x10 and the second at
// +0x14 (arguments loaded into EAX and EDX before the stores). Byte-for-byte
// twin of sub_004cecd0; the class identity of the two receivers is not
// established, so they stay separate objects.
class Object004cecf0 {
public:
    void sub_004cecf0(unsigned int first, unsigned int second);
};

// FUNCTION: RBR 0x004ce380
float Object004ce380::sub_004ce380() {
    return (float)field_50;
}

// FUNCTION: RBR 0x004ce3d0
extern "C" Record004ce3d0 * __stdcall sub_004ce3d0(unsigned int index) {
    return gRecords007186c0 + index;
}

// FUNCTION: RBR 0x004ce3f0
extern "C" int __stdcall sub_004ce3f0(unsigned int value) {
    for (int i = 0; i < 0x50; ++i) {
        if (value == gTable007a5820[i]) {
            return 0;
        }
    }
    return 1;
}

// FUNCTION: RBR 0x004ce9a0
void Object004ce9a0::sub_004ce9a0(void *record) {
    unsigned int index = *(unsigned int *)record;
    unsigned int value = *(unsigned int *)((char *)record + 0x98);
    *(unsigned int *)((char *)this + 0xb0 + index * 0x2b0) = value;
}

// FUNCTION: RBR 0x004ceaa0
Object004ceaa0::Object004ceaa0() {}

// FUNCTION: RBR 0x004ceb90
unsigned int Object004ceb90::sub_004ceb90(unsigned int selector,
                                         unsigned int index) {
    if (selector == 0) {
        return *(unsigned int *)((char *)this + 0xb00 + index * 4);
    }
    if (selector == 1) {
        return *(unsigned int *)((char *)this + 0xb08 + index * 4);
    }
    return *(unsigned int *)((char *)this + 0xb10 + index * 4);
}

// FUNCTION: RBR 0x004cecd0
void Object004cecd0::sub_004cecd0(unsigned int first, unsigned int second) {
    *(unsigned int *)((char *)this + 0x10) = first;
    *(unsigned int *)((char *)this + 0x14) = second;
}

// FUNCTION: RBR 0x004cecf0
void Object004cecf0::sub_004cecf0(unsigned int first, unsigned int second) {
    *(unsigned int *)((char *)this + 0x10) = first;
    *(unsigned int *)((char *)this + 0x14) = second;
}
