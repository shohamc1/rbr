// sub_00421de0: EXE 0x00421de0, 93 bytes. MSVC 7.1 (/O2 /Zl).
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
