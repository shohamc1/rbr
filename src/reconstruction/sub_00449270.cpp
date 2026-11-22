// sub_00449270: EXE 0x00449270, 25 bytes. Finalizer body of the class whose
// deleting finalizer sits at 0x004492f0 in vtable slot 0 of 0x0070e988. The
// layout is a polymorphic head at +0 (vtable slot restored here) followed by
// an intrusive-list node base at +4: node links at +4/+8 and the owning list
// pointer at +0xc. When the object still owns a list, the body decrements the
// counter at list+8 and tail-jumps into the 93-byte external unlink method
// sub_00447710 on the node base (this + 4). Binding the node base as a
// reference before the test pins the compiler's schedule: vtable store, owner
// load, this-adjustment, test. Sibling bodies: sub_004491e0 (vtable
// 0x0070e964, unlink sub_004477b0), sub_00449200 (0x0070e970, sub_00447850),
// sub_00449250 (0x0070e97c, sub_004478f0) — the four look like template
// instantiations of one wrapper. Compiled with MSVC 7.1 (/O2 /Zl).
// Raw-address vtable store: no object-local vtable is emitted.

extern "C" const void* const gVtable0070e988[]; // EXE .rdata 0x0070e988

class Head00449270 {
public:
    unsigned int field_00;   // vtable slot; 0x0070e988 for this class
};

class Node00449270 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_00447710();   // external unlink method, EXE 0x00447710
};

class Object00449270 : public Head00449270, public Node00449270 {
public:
    void sub_00449270();
};

void Object00449270::sub_00449270() {
    Head00449270::field_00 = (unsigned int)gVtable0070e988;
    Node00449270 &node = *static_cast<Node00449270 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_00447710();
    }
}
