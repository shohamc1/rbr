// sub_004491e0: EXE 0x004491e0, 25 bytes. Finalizer body of the class whose
// deleting finalizer sub_00449290 sits in vtable slot 0 of 0x0070e964. The
// layout is a polymorphic head at +0 (vtable slot restored here) followed by
// an intrusive-list node base at +4: node links at +4/+8 and the owning list
// pointer at +0xc. When the object still owns a list, the body decrements the
// counter at list+8 and tail-jumps into the 93-byte external unlink method
// sub_004477b0 on the node base (this + 4). Binding the node base as a
// reference before the test pins the compiler's schedule: vtable store, owner
// load, this-adjustment, test. Sibling bodies: sub_00449200 (vtable
// 0x0070e970, unlink sub_00447850), sub_00449250 (0x0070e97c, sub_004478f0),
// sub_00449270 (0x0070e988, sub_00447710) — the four look like template
// instantiations of one wrapper. Compiled with MSVC 7.1 (/O2 /Zl).
// Raw-address vtable store: no object-local vtable is emitted.

class Head004491e0 {
public:
    unsigned int field_00;   // vtable slot; 0x0070e964 for this class
};

class Node004491e0 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_004477b0();   // external unlink method, EXE 0x004477b0
};

class Object004491e0 : public Head004491e0, public Node004491e0 {
public:
    void sub_004491e0();
};

void Object004491e0::sub_004491e0() {
    Head004491e0::field_00 = 0x70e964;
    Node004491e0 &node = *static_cast<Node004491e0 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_004477b0();
    }
}
