// sub_004a5f10: EXE 0x004a5f10, 50 bytes. MSVC 7.1 (/O2 /Zl).
//
// thiscall push_back (RET 4): appends a node at the tail of an intrusive
// doubly-linked list. Container layout (batch-100 allocator family): first
// +0, last +4, count +8; nodes: next +4, prev +8, owner +0xc. The new node
// always gets prev = old last and next = 0; a non-null old last is forwarded
// to the new node, otherwise the node also becomes the container first. The
// node's owner is the container and the count is incremented in both paths.
// No incoming references in the Ghidra index; an embedded instance of this
// container at +0x208 is driven by sub_004a5fb0, which inlines this body.

struct Node004a5f10 {
    unsigned int field_00;
    Node004a5f10* field_04; // next
    Node004a5f10* field_08; // prev
    void* field_0c;         // owning container
};

class Object004a5f10 {
public:
    Node004a5f10* field_00; // first
    Node004a5f10* field_04; // last
    unsigned int field_08;  // count
    void sub_004a5f10(Node004a5f10* node);
};

void Object004a5f10::sub_004a5f10(Node004a5f10* node)
{
    Node004a5f10* last = field_04;
    node->field_08 = last;
    node->field_04 = 0;
    if (last != 0) {
        last->field_04 = node;
        field_04 = node;
    } else {
        field_00 = node;
        field_04 = node;
    }
    node->field_0c = this;
    field_08++;
}
