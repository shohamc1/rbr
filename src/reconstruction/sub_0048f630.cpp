// sub_0048f630: EXE 0x0048f630, 19 bytes. Finalizer body of an intrusive-list
// wrapper with the same shape as sub_0048f5c0: polymorphic head at +0, 3-pointer
// link node at +4 with the owning list pointer at +0xc. When the object still
// owns a list, the body decrements the counter at list+8 and tail-jumps into
// the 93-byte external unlink method sub_004544b0 on the node base (this + 4).
// sub_0048f600/sub_0048f630 share the node class Node0048f600 so both emit one
// mangled callee symbol for the manifest entry.
// Compiled with MSVC 7.1 (/O2 /Zl).

class Head0048f630 {
public:
    unsigned int field_00;   // polymorphic head, vtable slot
};

class Node0048f600 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_004544b0();   // external unlink method, EXE 0x004544b0
};

class Object0048f630 : public Head0048f630, public Node0048f600 {
public:
    void sub_0048f630();
};

void Object0048f630::sub_0048f630() {
    void *owner = field_08;
    Node0048f600 &node = *static_cast<Node0048f600 *>(this);
    if (owner != 0) {
        --*(int *)((char *)owner + 8);
        node.sub_004544b0();
    }
}
