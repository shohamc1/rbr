// sub_00420c30: MSVC 7.1 (/O2 /Zl).
//
// Destructor thunk of a class whose only member needing destruction is the
// 0x1c-byte MSVCP71 basic_string embedded at offset +4 (four bytes of plain
// data before it). MSVC emits `add ecx,4; jmp [__imp_??1?$basic_string@...]`
// (IAT slot 0x0070c2d8), the same shape as the matched sub_004c4170 thunk
// (string at +8 there). Reached as a member destructor at 0x00704b5c,
// 0x0070556c and 0x0070557c. The string import declaration is in msvcp71.h.

#include "msvcp71.h"

class Object00420c30 {
public:
    unsigned char unknown_00[4];
    std::string field_04;
    ~Object00420c30();
};

// sub_00420c40: MSVC 7.1 (/O2 /Zl).
//
// Destructor thunk identical in shape to sub_00420c30: the class embeds the
// 0x1c-byte MSVCP71 basic_string at offset +4 (four bytes of plain data
// before it), so MSVC emits `add ecx,4; jmp [__imp_??1?$basic_string@...]`
// (IAT slot 0x0070c2d8). Reached as a member destructor at 0x00705574 and
// 0x00705584, next to the sub_00420c30 call sites. The string import
// declaration is in msvcp71.h.

#include "msvcp71.h"

class Object00420c40 {
public:
    unsigned char unknown_00[4];
    std::string field_04;
    ~Object00420c40();
};

// sub_004213b0: MSVC 7.1 (/O2 /Zl).
//
// Recursive release of the hierarchical intrusive list built from the node
// class whose string destructor is sub_00421ff0. The node keeps its sibling
// chain at +0, a backlink at +4 and its child chain at +8, the 0x1c-byte
// MSVCP71 basic_string at +0x10, a byte flag at +0x2c and the sentinel flag
// byte at +0x2d (same shape as the sub_00499270 family, whose sentinels sit
// at +0x1d). __thiscall on the container in ECX with the head node as the
// one stack argument: while the walker is not the sentinel, release its
// child chain recursively, advance the walker to the sibling at +0, destroy
// the lagging node's embedded string through sub_00421ff0, free it through
// the cdecl operator-delete thunk sub_006efdd0, then adopt the walker as
// the next lagging node. Called from sub_00421720 at 0x00421742.
//
// The class below reuses the Object00421ff0 name so the destructor call
// mangles to the same ??1 symbol that Unit00421ff0.cpp defines; the field
// view here is the richer one shared by this family.
//
// Register encoding: the parameter itself must be the node that lags one
// step behind the walker (destroyed and freed each iteration, refreshed at
// the body end), while a separate `current` walker is initialized from it;
// reusing the parameter for the lagging node pins the entry to
// `mov edi,[esp+0x10]; mov esi,edi` with the upfront PUSH EDI, where a
// second local swaps the ESI/EDI roles. An explicit destructor call on a
// class-typed parameter keeps the deleting-dtor COMDAT unemitted.

#include "msvcp71.h"

class Object00421ff0 {
public:
    Object00421ff0* field_00;  // sibling chain, +0
    void* field_04;            // backlink, +4
    Object00421ff0* field_08;  // child chain, +8
    unsigned char field_0c[4];
    std::string field_10;      // destroyed by sub_00421ff0
    unsigned char field_2c;
    unsigned char flag_2d;     // sentinel flag
    ~Object00421ff0();
};

extern "C" void __cdecl sub_006efdd0(void* memory);  // 0x006efdd0, operator delete thunk

typedef char node004213b0_size_assertion[(sizeof(Object00421ff0) == 0x30) ? 1 : -1];
typedef char node004213b0_flag_assertion[
    (((unsigned long)&((Object00421ff0*)0)->flag_2d) == 0x2d) ? 1 : -1];

class Object004213b0 {
public:
    void sub_004213b0(Object00421ff0* node);
};

// sub_00421840: MSVC 7.1 (/O2 /Zl).
//
// Link step of the hierarchical intrusive list of sub_004213b0 (node links
// +0/+4/+8, sentinel flag byte +0x2d). __thiscall on the node being spliced
// (ECX) with the container record as the one stack argument; the container's
// +4 field is the list record whose +4 field is the head link. The node at
// this->field_08 is linked into the position this occupies: this->field_08
// takes that node's +0 link, a non-sentinel +0 link target's backlink (+4)
// is repointed at this, the node's backlink takes this's backlink (this->field_04),
// and the record that referenced this (list head at list->field_04, or the +0
// link of the node at this->field_04, or its +8 link) is repointed at the
// node, whose +0 link becomes this and whose backlink becomes the node.
// Called from sub_00421a70 at 0x00421c08, 0x00421c5a, 0x00421cbe and from
// sub_004213f0 at 0x004214f3; sub_004218a0 is the mirror operation with the
// +0/+8 link roles swapped.

class Object00421840 {
public:
    Object00421840* field_00;  // sibling-side link, +0
    Object00421840* field_04;  // backlink, +4
    Object00421840* field_08;  // child-side link, +8
    unsigned char field_0c[0x20];
    unsigned char field_2c;
    unsigned char flag_2d;     // sentinel flag

    void sub_00421840(Object00421840* container);
};

// sub_004218a0: MSVC 7.1 (/O2 /Zl).
//
// Mirror of sub_00421840 on the same hierarchical intrusive list (node links
// +0/+4/+8, sentinel flag byte +0x2d): every +0 link of that function is a
// +8 link here and vice versa, while the backlink (+4) and the list-head
// check at container->field_04->field_04 keep their slots. __thiscall on the
// node being spliced (ECX) with the container record as the one stack
// argument. The node at this->field_00 is linked into the position this
// occupies: this->field_00 takes that node's +8 link, a non-sentinel +8
// link target's backlink is repointed at this, the node's backlink takes
// this's backlink, and the record that referenced this (list head, or the +8
// link of the node at this->field_04, or its +0 link) is repointed at the
// node, whose +8 link becomes this and whose backlink becomes the node.
// Called from sub_00421a70 at 0x00421c3c, 0x00421c72, 0x00421cda and from
// sub_004213f0 at 0x0042155f.

class Object004218a0 {
public:
    Object004218a0* field_00;  // sibling-side link, +0
    Object004218a0* field_04;  // backlink, +4
    Object004218a0* field_08;  // child-side link, +8
    unsigned char field_0c[0x20];
    unsigned char field_2c;
    unsigned char flag_2d;     // sentinel flag

    void sub_004218a0(Object004218a0* container);
};

// sub_00421a10: MSVC 7.1 (/O2 /Zl).
//
// Cursor advance over the hierarchical intrusive list of sub_004213b0 (node
// links +0/+4/+8, sentinel flag byte +0x2d). __fastcall: the first (ECX)
// parameter is unused and the second (EDX) is a pointer to the cursor slot;
// the caller sub_00421a70 passes &slot at 0x00421ad0 after `lea edx,[esp+0x6c]`.
// If the current node is the sentinel, nothing happens. Otherwise, when the
// child link (+8) is live, the cursor descends the +0 chain from the child
// and keeps the last node whose +0 successor is the sentinel; when the child
// link is the sentinel, the cursor walks the +4 chain, adopting each node
// whose +8 link matches the cursor, and stops at the first flagged node or
// mismatch. The two 16-aligned loop heads carry `lea`-encoded two-byte
// alignment padding (0x00421a2d, 0x00421a49).
//
// NONMATCHING: 87 bytes, one instruction differs. Everything matches except
// the first loop's advance load at 0x00421a32: the reference is
// `mov eax,[ecx]` (8B 01, loading next through current's home) while every
// source shape tried makes MSVC 7.1 emit `mov eax,[eax]` (8B 00,
// rematerialized through the copy source). Shapes tried: while with
// current/next temps, swapped declaration order, loaded temp, C++ reference
// binding, child as the walker, double-dereference while condition, for(;;)
// with break, and a for-loop with the load in the condition.
// The int return with free-EAX return expressions (return (int)node/next/
// other emit no instruction) is load-bearing: with a void return MSVC homes
// the node in ECX and the child in EAX, inverting the whole entry and both
// paths.

struct Node00421a10 {
    Node00421a10* field_00;  // sibling-side chain, +0
    Node00421a10* field_04;  // +4 chain
    Node00421a10* field_08;  // child-side chain, +8
    unsigned char field_0c[0x20];
    unsigned char field_2c;
    unsigned char flag_2d;   // sentinel flag
};

// sub_00421de0: MSVC 7.1 (/O2 /Zl).
//
// Reverse cursor advance over the hierarchical intrusive list of sub_004213b0
// (node links +0/+4/+8, sentinel flag byte +0x2d), sibling of sub_00421a10.
// __fastcall: the first (ECX) parameter is unused and the second (EDX) is
// the cursor slot pointer; the caller sub_00421170 passes it at 0x004211e2.
// A sentinel current node moves the cursor to its +8 link and returns. When
// the +0 link is live, the cursor descends the +8 chain from it and keeps
// the last node whose +8 successor is the sentinel; when the +0 link is the
// sentinel, the cursor walks the +4 chain while each node's +0 link matches
// the cursor, adopting every match, and writes the final node back only if
// it is not the sentinel. The 16-aligned second loop head carries
// `lea`-encoded alignment padding at 0x00421e1d.
//
// As in sub_00421a10, the int return with free-EAX return expressions
// (return (int)advanced/next/other emit no instruction) is load-bearing for
// the register homes: it keeps the node in EAX from `mov eax,[edx]` through
// the path-2 `mov eax,[eax+4]`, and hoisting the early return value into a
// local avoids an aliasing reload after the cursor store.
//
// NONMATCHING: 93 bytes, one instruction differs — the same one-byte
// residual as sub_00421a10: the first loop's advance load at 0x00421e05 is
// `mov eax,[ecx+8]` (8B 41 08) in the reference and `mov eax,[eax+8]`
// (8B 40 08) in every compiled shape (while, guarded do-while, for-loop with
// the load in the condition, array-indexed base, merged or split walker
// variables, and optimize("g"/"s") pragmas all score 1 on this byte).

struct Node00421de0 {
    Node00421de0* field_00;  // sibling-side chain, +0
    Node00421de0* field_04;  // +4 chain
    Node00421de0* field_08;  // child-side chain, +8
    unsigned char field_0c[0x20];
    unsigned char field_2c;
    unsigned char flag_2d;   // sentinel flag
};

// FUNCTION: RBR 0x00420c30
Object00420c30::~Object00420c30() {
}

// FUNCTION: RBR 0x00420c40
Object00420c40::~Object00420c40() {
}

// FUNCTION: RBR 0x004213b0
void Object004213b0::sub_004213b0(Object00421ff0* node) {
    Object00421ff0* current = node;
    while (current->flag_2d == 0) {
        sub_004213b0(current->field_08);
        current = current->field_00;
        node->~Object00421ff0();
        sub_006efdd0(node);
        node = current;
    }
}

// FUNCTION: RBR 0x00421840
void Object00421840::sub_00421840(Object00421840* container) {
    Object00421840* promoted = field_08;
    field_08 = promoted->field_00;
    if (promoted->field_00->flag_2d == 0) {
        promoted->field_00->field_04 = this;
    }
    promoted->field_04 = field_04;
    Object00421840* list = container->field_04;
    if (this == list->field_04) {
        list->field_04 = promoted;
        promoted->field_00 = this;
        field_04 = promoted;
    } else if (this == field_04->field_00) {
        field_04->field_00 = promoted;
        promoted->field_00 = this;
        field_04 = promoted;
    } else {
        field_04->field_08 = promoted;
        promoted->field_00 = this;
        field_04 = promoted;
    }
}

// FUNCTION: RBR 0x004218a0
void Object004218a0::sub_004218a0(Object004218a0* container) {
    Object004218a0* promoted = field_00;
    field_00 = promoted->field_08;
    if (promoted->field_08->flag_2d == 0) {
        promoted->field_08->field_04 = this;
    }
    promoted->field_04 = field_04;
    Object004218a0* list = container->field_04;
    if (this == list->field_04) {
        list->field_04 = promoted;
        promoted->field_08 = this;
        field_04 = promoted;
    } else if (this == field_04->field_08) {
        field_04->field_08 = promoted;
        promoted->field_08 = this;
        field_04 = promoted;
    } else {
        field_04->field_00 = promoted;
        promoted->field_08 = this;
        field_04 = promoted;
    }
}

// NONMATCHING
// FUNCTION: RBR 0x00421a10
extern "C" int __fastcall sub_00421a10(void* unused, Node00421a10** cursor) {
    Node00421a10* node = *cursor;
    if (node->flag_2d != 0) {
        return (int)node;
    }
    Node00421a10* child = node->field_08;
    if (child->flag_2d == 0) {
        Node00421a10* current = child;
        Node00421a10* next = current->field_00;
        while (next->flag_2d == 0) {
            current = next;
            next = current->field_00;
        }
        *cursor = current;
        return (int)next;
    }
    Node00421a10* other = node->field_04;
    if (other->flag_2d == 0) {
        do {
            if (*cursor != other->field_08) {
                break;
            }
            *cursor = other;
            other = other->field_04;
        } while (other->flag_2d == 0);
    }
    *cursor = other;
    return (int)other;
}

// NONMATCHING
// FUNCTION: RBR 0x00421de0
extern "C" int __fastcall sub_00421de0(void* unused, Node00421de0** cursor) {
    Node00421de0* node = *cursor;
    if (node->flag_2d != 0) {
        Node00421de0* advanced = node->field_08;
        *cursor = advanced;
        return (int)advanced;
    }
    Node00421de0* current = node->field_00;
    if (current->flag_2d == 0) {
        Node00421de0* next = current->field_08;
        while (next->flag_2d == 0) {
            current = next;
            next = current->field_08;
        }
        *cursor = current;
        return (int)next;
    }
    Node00421de0* other = node->field_04;
    while (other->flag_2d == 0 && (Node00421de0*)*cursor == other->field_00) {
        *cursor = other;
        other = other->field_04;
    }
    if (other->flag_2d == 0) {
        *cursor = other;
    }
    return (int)other;
}
