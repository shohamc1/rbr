// sub_0048f5e0: EXE 0x0048f5e0, 25 bytes. Finalizer body of the class whose
// deleting finalizer sub_0048f670 sits in vtable slot 0 of 0x00712f70 (the
// table also holds deleting finalizers in slots 3, 6, and 9, e.g. sub_0048f810
// at slot 9 whose body is sub_0048f600). The layout is a polymorphic head at +0
// (vtable slot restored here) followed by a 3-pointer link node at +4: node
// links at +4/+8 and the owning list pointer at +0xc. When the object still
// owns a list, the body decrements the counter at list+8 and tail-jumps into
// the 93-byte external unlink method sub_00454ab0 on the node base (this + 4);
// the wrappers of that unlink share the node class Node0048f4f0 for one
// mangled callee symbol. Binding the node base as a reference pins the
// vtable-store/owner-load/this-adjustment schedule. Raw-address vtable store:
// no object-local vtable is emitted.
// Compiled with MSVC 7.1 (/O2 /Zl).

extern "C" const void* const gVtable00712f70[]; // EXE .rdata 0x00712f70

class Head0048f5e0 {
public:
    unsigned int field_00;   // vtable slot; 0x00712f70 for this class
};

class Node0048f4f0 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_00454ab0();   // external unlink method, EXE 0x00454ab0
};

class Object0048f5e0 : public Head0048f5e0, public Node0048f4f0 {
public:
    void sub_0048f5e0();
};

void Object0048f5e0::sub_0048f5e0() {
    Head0048f5e0::field_00 = (unsigned int)gVtable00712f70;
    Node0048f4f0 &node = *static_cast<Node0048f4f0 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_00454ab0();
    }
}
