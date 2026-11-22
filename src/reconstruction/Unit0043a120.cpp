// sub_0043a120: Compiled with MSVC 7.1 (/O2 /Zl).
//
// thiscall accessor of a vector-like container whose element pointers sit at
// +4 (first) and +8 (last). Returns 0 when the container is empty, otherwise
// the element count (last - first with 4-byte elements: SUB then SAR 2).
// The storage-end sibling at +0xc belongs to sub_0043a140. No EXE caller or
// address reference is known yet; the convention comes from the ECX-only
// register input.
#include "layout.h"

class Object0043a120 {
public:
    int sub_0043a120();
    unsigned char unknown_00[0x4];
    unsigned int* field_4;         // first element
    unsigned int* field_8;         // last element
};

LAYOUT_OFFSET(Object0043a120, field_4, 0x4);
LAYOUT_OFFSET(Object0043a120, field_8, 0x8);

// sub_0043a140: Compiled with MSVC 7.1 (/O2 /Zl).
//
// thiscall accessor of the same vector-like container family as
// sub_0043a120, but the difference runs from +4 (first) to +0xc (storage
// end): the allocated element count. Returns 0 when the container is empty.
// No EXE caller or address reference is known yet; the convention comes from
// the ECX-only register input.
#include "layout.h"

class Object0043a140 {
public:
    int sub_0043a140();
    unsigned char unknown_00[0x4];
    unsigned int* field_4;         // first element
    unsigned char unknown_08[0x4];
    unsigned int* field_c;         // storage end
};

LAYOUT_OFFSET(Object0043a140, field_4, 0x4);
LAYOUT_OFFSET(Object0043a140, field_c, 0xc);

// sub_0043a200: Compiled with MSVC 7.1 (/O2 /Zl).
//
// Deallocation helper for the 4-byte-element allocator family of
// sub_0043a290: forwards the first argument to the operator-delete thunk
// sub_006efdd0 (jmp [__imp_??3@YAXPAX@Z] at 0x006efdd0) and ignores the
// second argument, the element count. __stdcall with two words of
// arguments (RET 8); ECX is never read. The Ghidra packet drops the
// POP ECX/RET 8 tail; the raw EXE bytes carry the full 14-byte body.
extern "C" void __cdecl sub_006efdd0(void* block);   // operator delete thunk

// sub_0043a210: Compiled with MSVC 7.1 (/O2 /Zl).
//
// thiscall cursor advance: adds count elements (LEA with scale 4, memory
// ADD) to the pointer at +0 and returns the receiver (MOV EAX,ECX entry
// home; EAX is this at RET 4). Pairs with sub_0043a250, which stores the
// same pointer and also returns the receiver, and sub_0043a260 (pointer
// difference as an element count); +0 is a cursor over 4-byte elements.
// No EXE caller known; the convention comes from ECX input with one stack
// argument (RET 4).
#include "layout.h"

class Object0043a210 {
public:
    Object0043a210* sub_0043a210(unsigned int count);
    unsigned int* field_0;           // cursor over 4-byte elements
};

LAYOUT_OFFSET(Object0043a210, field_0, 0x0);

// sub_0043a250: Compiled with MSVC 7.1 (/O2 /Zl).
//
// thiscall cursor store: writes the stack argument to the pointer at +0 and
// returns the receiver (MOV EAX,ECX entry home; EAX is this at RET 4).
// Pairs with sub_0043a210 (advance, also returning the receiver) and
// sub_0043a260 (difference); +0 is the cursor over 4-byte elements. No EXE
// caller known; the convention comes from ECX input with one stack argument
// (RET 4).
#include "layout.h"

class Object0043a250 {
public:
    Object0043a250* sub_0043a250(unsigned int* pointer);
    unsigned int* field_0;           // cursor over 4-byte elements
};

LAYOUT_OFFSET(Object0043a250, field_0, 0x0);

// sub_0043a260: Compiled with MSVC 7.1 (/O2 /Zl).
//
// thiscall cursor difference: (this->cursor - other->cursor) as a 4-byte
// element count (SUB with a memory operand, then SAR 2). The argument is
// dereferenced, so it is the same cursor-holding record, not a raw pointer.
// Pairs with sub_0043a210/sub_0043a250. No EXE caller known; the convention
// comes from ECX input with one stack argument (RET 4).
#include "layout.h"

class Object0043a260 {
public:
    int sub_0043a260(Object0043a260* other);
    unsigned int* field_0;           // cursor over 4-byte elements
};

LAYOUT_OFFSET(Object0043a260, field_0, 0x0);

// sub_0043a270: Compiled with MSVC 7.1 (/O2 /Zl).
//
// Fill loop for 4-byte elements: copies *value into every slot of
// [first, last). The value is reloaded from *value on every iteration
// (MOV ESI,[EDX] inside the loop), so value may alias the filled range.
// Callers at 0x0043a74e and 0x0043a782 push three pointers and clean up
// with ADD ESP,0xc: cdecl. Part of the allocator/vector helper family
// around sub_0043a290/sub_0043a2c0.

// sub_0043a290: Compiled with MSVC 7.1 (/O2 /Zl).
//
// Allocation helper for 4-byte elements: operator new(count * 4), returning
// the block. sub_006efa32 is the pinned operator new (malloc/_callnewh/
// std::_Nomemory retry loop). Pairs with sub_0043a200, which hands the
// block back through the operator-delete thunk sub_006efdd0. cdecl with a
// second unused argument: a tail jump into sub_006efa32 is impossible while
// the caller frame carries an argument nobody cleans, and the reference
// keeps PUSH/CALL/ADD ESP,4/RET. The (count, hint) shape matches the
// allocator family of sub_0043a200 (two arguments, RET 8).
extern "C" void* __cdecl sub_006efa32(unsigned int size);   // operator new

// sub_0043a2c0: Compiled with MSVC 7.1 (/O2 /Zl).
//
// Backward move for 4-byte elements: memmove(dst_end - count, first,
// count * 4) with count = last - first. The element count is computed once
// (SUB, SAR 2), scaled once (SHL 2), and the byte size is reused for both
// the destination and the size argument. The memmove import is called
// through its IAT slot (0x0070c510) as a dllimport. cdecl.
extern "C" __declspec(dllimport) void* __cdecl memmove(void*, const void*,
                                                       unsigned int);

// FUNCTION: RBR 0x0043a120
int Object0043a120::sub_0043a120() {
    if (field_4 == 0) {
        return 0;
    }
    return field_8 - field_4;
}

// FUNCTION: RBR 0x0043a140
int Object0043a140::sub_0043a140() {
    if (field_4 == 0) {
        return 0;
    }
    return field_c - field_4;
}

// FUNCTION: RBR 0x0043a200
extern "C" void __stdcall sub_0043a200(void* pointer, unsigned int) {
    sub_006efdd0(pointer);
}

// FUNCTION: RBR 0x0043a210
Object0043a210* Object0043a210::sub_0043a210(unsigned int count) {
    field_0 += count;
    return this;
}

// FUNCTION: RBR 0x0043a250
Object0043a250* Object0043a250::sub_0043a250(unsigned int* pointer) {
    field_0 = pointer;
    return this;
}

// FUNCTION: RBR 0x0043a260
int Object0043a260::sub_0043a260(Object0043a260* other) {
    return field_0 - other->field_0;
}

// FUNCTION: RBR 0x0043a270
extern "C" void __cdecl sub_0043a270(unsigned int* first, unsigned int* last,
                                      const unsigned int* value) {
    while (first != last) {
        *first = *value;
        ++first;
    }
}

// FUNCTION: RBR 0x0043a290
extern "C" void* __cdecl sub_0043a290(unsigned int count, const void*) {
    return sub_006efa32(count * 4);
}

// FUNCTION: RBR 0x0043a2c0
extern "C" void __cdecl sub_0043a2c0(const unsigned int* first,
                                     const unsigned int* last,
                                     unsigned int* dst_end) {
    int count = last - first;
    memmove(dst_end - count, first, count * 4);
}
