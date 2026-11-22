// sub_00453b50: MSVC 7.1 (/O2 /Zl).
//
// thiscall member, no stack args, bare RET. Closes a pending gap in an
// intrusive doubly-linked list (node links: next +4, prev +8, owner +0xc;
// container: first +0, last +4, count +8 — the batch-100 allocator layout).
// The receiver holds the node after the pending range (field_00), the node
// before it (field_04), and the owning container (field_08). Where both
// neighbours exist they are linked directly (B.next = A, A.prev = B); a null
// head-side node makes field_00 the container first; a null tail-side node
// makes field_04 the container last with a nulled next; with neither, the
// container is emptied. All three receiver fields are cleared in every path.
// EXE 0x00454870 (stub in config/symbols.csv; ) holds a byte-identical 93-byte second
// copy of this body.

struct Node00453b50 {
    unsigned int field_00;
    Node00453b50* field_04; // next toward the tail
    Node00453b50* field_08; // prev toward the head
};

struct Container00453b50 {
    Node00453b50* field_00; // first
    Node00453b50* field_04; // last
    unsigned int field_08;  // count
};

class Object00453b50 {
public:
    Node00453b50* field_00;
    Node00453b50* field_04;
    Container00453b50* field_08;
    void sub_00453b50();
};

// FUNCTION: RBR 0x00453b50
void Object00453b50::sub_00453b50()
{
    if (field_08 != 0) {
        if (field_00 != 0) {
            field_00->field_08 = field_04;
            if (field_04 != 0)
                field_04->field_04 = field_00;
            else
                field_08->field_00 = field_00;
        } else if (field_04 != 0) {
            field_04->field_04 = 0;
            field_08->field_04 = field_04;
        } else {
            field_08->field_00 = 0;
            field_08->field_04 = 0;
        }
    }
    field_00 = 0;
    field_04 = 0;
    field_08 = 0;
}
