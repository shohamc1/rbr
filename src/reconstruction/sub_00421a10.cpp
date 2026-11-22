// sub_00421a10: EXE 0x00421a10, 87 bytes. MSVC 7.1 (/O2 /Zl).
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
