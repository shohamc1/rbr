// sub_004a5fb0: EXE 0x004a5fb0, 59 bytes. MSVC 7.1 (/O2 /Zl).
//
// thiscall member (RET 4): appends a node at the tail of the intrusive list
// embedded at +0x208 of the receiver. The body is the sub_004a5f10 push_back
// inlined with its container `this' adjusted to receiver+0x208 (the count at
// +0x210 and the node's owner both use the adjusted address); the old last is
// read through the unadjusted receiver (+0x20c) before the adjustment. Node
// layout: next +4, prev +8, owner +0xc. No incoming references in the Ghidra
// index.

struct Node004a5fb0 {
    unsigned int field_00;
    Node004a5fb0* field_04; // next
    Node004a5fb0* field_08; // prev
    void* field_0c;         // owning container
};

class List004a5fb0 {
public:
    Node004a5fb0* field_00; // first
    Node004a5fb0* field_04; // last
    unsigned int field_08;  // count
};

class Object004a5fb0 {
public:
    unsigned char field_00[0x20c];
    Node004a5fb0* field_20c; // last node of the embedded list
    void sub_004a5fb0(Node004a5fb0* node);
};

void Object004a5fb0::sub_004a5fb0(Node004a5fb0* node)
{
    Node004a5fb0* last = field_20c;
    List004a5fb0* self = (List004a5fb0*)((unsigned char*)this + 0x208);
    node->field_08 = last;
    node->field_04 = 0;
    if (last != 0) {
        last->field_04 = node;
        self->field_04 = node;
    } else {
        self->field_00 = node;
        self->field_04 = node;
    }
    node->field_0c = self;
    self->field_08++;
}
