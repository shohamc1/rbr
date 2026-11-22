// Object004b04b0 member: EXE 0x004b04b0, 48 bytes. __thiscall with one
// pointer stack argument (RET 4), void return.
// Tail-append for an intrusive doubly linked list. List header: first
// node +0, last node +4, count +8 (zeroed by the 0x004b04a0-family
// constructors). Node: next +0, prev +4, owner +8. The new node's
// prev/next stores are hoisted above the branch (prev takes the old
// last, which is zero in the empty case); MSVC tail-duplicates the
// owner/count/RET suffix into both paths.
// Byte-identical to sub_004b04f0 at 0x004b04f0.

struct Node004b04b0 {
    Node004b04b0 *field_00_next;
    Node004b04b0 *field_04_prev;
    void *field_08_owner;
};

struct Object004b04b0 {
    Node004b04b0 *field_00_first;
    Node004b04b0 *field_04_last;
    unsigned int field_08_count;
    void sub_004b04b0(Node004b04b0 *node);
};

void Object004b04b0::sub_004b04b0(Node004b04b0 *node) {
    Node004b04b0 *last = field_04_last;
    node->field_04_prev = last;
    node->field_00_next = 0;
    if (last != 0) {
        last->field_00_next = node;
        field_04_last = node;
    } else {
        field_00_first = node;
        field_04_last = node;
    }
    node->field_08_owner = this;
    ++field_08_count;
}
