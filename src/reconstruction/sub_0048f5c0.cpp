// sub_0048f5c0: EXE 0x0048f5c0, 19 bytes. Finalizer body of an intrusive-list
// wrapper in the sub_0048cc00 logging neighborhood. The layout is a polymorphic
// head at +0 followed by a 3-pointer link node at +4: node links at +4/+8 and
// the owning list pointer at +0xc. When the object still owns a list, the body
// decrements the counter at list+8 and tail-jumps into the 93-byte external
// unlink method sub_00454ab0 on the node base (this + 4). The unlink body is
// byte-identical to the thiscall unlink sub_0043d920; the wrappers of
// sub_00454ab0 (sub_0048f4f0, sub_0048f500, sub_0048f5c0, sub_0048f5e0) model
// it as a member of the shared node class Node0048f4f0 so one mangled callee
// symbol serves the manifest entry.
// Compiled with MSVC 7.1 (/O2 /Zl).

class Head0048f5c0 {
public:
    unsigned int field_00;   // polymorphic head, vtable slot
};

class Node0048f4f0 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_00454ab0();   // external unlink method, EXE 0x00454ab0
};

class Object0048f5c0 : public Head0048f5c0, public Node0048f4f0 {
public:
    void sub_0048f5c0();
};

void Object0048f5c0::sub_0048f5c0() {
    void *owner = field_08;
    Node0048f4f0 &node = *static_cast<Node0048f4f0 *>(this);
    if (owner != 0) {
        --*(int *)((char *)owner + 8);
        node.sub_00454ab0();
    }
}
