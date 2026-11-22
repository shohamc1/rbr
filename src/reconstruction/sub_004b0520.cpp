// Object004b0520 member: EXE 0x004b0520, 50 bytes. __thiscall with one
// pointer stack argument (RET 4), void return.
// Tail-append for the prefixed-node intrusive list family: the list
// header keeps first +0, last +4, count +8 (16-byte header constructed
// by sub_004b04a0/sub_004b04e0), while nodes carry a dword prefix at +0
// before the link block: next +4, prev +8, owner +0xc — the same node
// shape as the wave-1 intrusive-list wrapper family. Two bytes longer
// than the bare-node sub_004b04b0 because the larger displacements
// (+0xc owner, [EDX+4] next) need full encodings.

struct Node004b0520 {
    unsigned int field_00;
    Node004b0520 *field_04_next;
    Node004b0520 *field_08_prev;
    void *field_0c_owner;
};

struct Object004b0520 {
    Node004b0520 *field_00_first;
    Node004b0520 *field_04_last;
    unsigned int field_08_count;
    void sub_004b0520(Node004b0520 *node);
};

void Object004b0520::sub_004b0520(Node004b0520 *node) {
    Node004b0520 *last = field_04_last;
    node->field_08_prev = last;
    node->field_04_next = 0;
    if (last != 0) {
        last->field_04_next = node;
        field_04_last = node;
    } else {
        field_00_first = node;
        field_04_last = node;
    }
    node->field_0c_owner = this;
    ++field_08_count;
}
