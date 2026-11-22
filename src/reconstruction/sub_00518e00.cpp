// sub_00518e00: EXE 0x00518e00, 25 bytes. Finalizer body of the class whose
// deleting finalizer sub_00518ee0 sits in vtable slot 0 of 0x00720fe8. The
// layout is a polymorphic head at +0 (vtable slot restored here) followed by
// an intrusive-list node base at +4: node links at +4/+8 and the owning list
// pointer at +0xc. When the object still owns a list, the body decrements the
// counter at list+8 and tail-jumps into the 93-byte external unlink method
// sub_00453310 on the node base (this + 4). Binding the node base as a
// reference before the test pins the compiler's schedule: vtable store, owner
// load, this-adjustment, test. Sibling bodies: sub_00518e20 (vtable
// 0x00720ff4, unlink sub_004536d0), plus unclaimed bodies around
// sub_00518e40-sub_00518e80 — the family looks like template instantiations
// of one wrapper, mirroring the sub_004491e0 family. Compiled with
// MSVC 7.1 (/O2 /Zl). Raw-address vtable store: no object-local vtable is
// emitted.

extern "C" const void* const gVtable00720fe8[]; // EXE .rdata 0x00720fe8

class Head00518e00 {
public:
    unsigned int field_00;   // vtable slot; 0x00720fe8 for this class
};

class Node00453310 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_00453310();   // external unlink method, EXE 0x00453310
};

class Object00518e00 : public Head00518e00, public Node00453310 {
public:
    void sub_00518e00();
};

void Object00518e00::sub_00518e00() {
    Head00518e00::field_00 = (unsigned int)gVtable00720fe8;
    Node00453310 &node = *static_cast<Node00453310 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_00453310();
    }
}
