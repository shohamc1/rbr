// sub_0048f4f0: EXE 0x0048f4f0, 16 bytes. Finalizer body of an intrusive-list
// wrapper in the sub_0048cc00 logging neighborhood. The node base sits at +0 of
// this wrapper: node links at +0/+4 and the owning list pointer at +8. When the
// object still owns a list, the body decrements the counter at list+8 and
// tail-jumps into the 93-byte external unlink method sub_00454ab0 on the node
// base. sub_0048f500 is a byte-identical sibling body with the same unlink
// target; sub_0048f5c0/sub_0048f5e0 call the same unlink on a node base
// embedded at +4. The unlink body at 0x00454ab0 is byte-identical to the
// thiscall unlink sub_0043d920, so all four wrappers model it as a member of
// one shared node class (one mangled callee symbol for the manifest).
// Compiled with MSVC 7.1 (/O2 /Zl).

class Node0048f4f0 {
public:
    void *field_00;   // node link, +0 in the wrapper
    void *field_04;   // node link, +4 in the wrapper
    void *field_08;   // owning list pointer, +8 in the wrapper
    void sub_00454ab0();   // external unlink method, EXE 0x00454ab0
};

class Object0048f4f0 : public Node0048f4f0 {
public:
    void sub_0048f4f0();
};

void Object0048f4f0::sub_0048f4f0() {
    Node0048f4f0 &node = *this;
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_00454ab0();
    }
}
