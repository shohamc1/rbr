// sub_00518e20: EXE 0x00518e20, 25 bytes. Finalizer body of the class whose
// deleting finalizer sub_00518f20 sits in vtable slot 0 of 0x00720ff4. The
// layout is a polymorphic head at +0 (vtable slot restored here) followed by
// an intrusive-list node base at +4: node links at +4/+8 and the owning list
// pointer at +0xc. When the object still owns a list, the body decrements the
// counter at list+8 and tail-jumps into the 93-byte external unlink method
// sub_004536d0 on the node base (this + 4). Binding the node base as a
// reference before the test pins the compiler's schedule: vtable store, owner
// load, this-adjustment, test. Sibling bodies: sub_00518e00 (vtable
// 0x00720fe8, unlink sub_00453310), plus unclaimed bodies around
// sub_00518e40-sub_00518e80 — the family looks like template instantiations
// of one wrapper, mirroring the sub_004491e0 family. Compiled with
// MSVC 7.1 (/O2 /Zl). Raw-address vtable store: no object-local vtable is
// emitted.

extern "C" const void* const gVtable00720ff4[]; // EXE .rdata 0x00720ff4

class Head00518e20 {
public:
    unsigned int field_00;   // vtable slot; 0x00720ff4 for this class
};

class Node004536d0 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_004536d0();   // external unlink method, EXE 0x004536d0
};

class Object00518e20 : public Head00518e20, public Node004536d0 {
public:
    void sub_00518e20();
};

void Object00518e20::sub_00518e20() {
    Head00518e20::field_00 = (unsigned int)gVtable00720ff4;
    Node004536d0 &node = *static_cast<Node004536d0 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_004536d0();
    }
}
