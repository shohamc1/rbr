// sub_0048f500: Byte-identical sibling of the
// finalizer body sub_0048f4f0 (same node layout at +0, same 93-byte external
// unlink sub_00454ab0, only the JMP immediate differs by the address gap).
// Both look like COMDAT copies of one template instantiation for two classes.
// The unlink is modeled as a member of the shared node class Node0048f4f0 so
// every wrapper of sub_00454ab0 emits one mangled callee symbol.
// Compiled with MSVC 7.1 (/O2 /Zl).

class Node0048f4f0 {
public:
    void *field_00;   // node link, +0 in the wrapper
    void *field_04;   // node link, +4 in the wrapper
    void *field_08;   // owning list pointer, +8 in the wrapper
    void sub_00454ab0();   // external unlink method, EXE 0x00454ab0
};

class Object0048f500 : public Node0048f4f0 {
public:
    void sub_0048f500();
};

// FUNCTION: RBR 0x0048f500
void Object0048f500::sub_0048f500() {
    Node0048f4f0 &node = *this;
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_00454ab0();
    }
}
