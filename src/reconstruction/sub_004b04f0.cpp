// Object004b04f0 member: EXE 0x004b04f0, 48 bytes. __thiscall with one
// pointer stack argument (RET 4), void return.
// Byte-identical tail-append to sub_004b04b0 at 0x004b04b0: intrusive
// doubly linked list with first +0, last +4, count +8 and nodes holding
// next +0, prev +4, owner +8. A separate emitted body implies a separate
// class or translation unit for the original build.

struct Node004b04f0 {
    Node004b04f0 *field_00_next;
    Node004b04f0 *field_04_prev;
    void *field_08_owner;
};

struct Object004b04f0 {
    Node004b04f0 *field_00_first;
    Node004b04f0 *field_04_last;
    unsigned int field_08_count;
    void sub_004b04f0(Node004b04f0 *node);
};

void Object004b04f0::sub_004b04f0(Node004b04f0 *node) {
    Node004b04f0 *last = field_04_last;
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
