// sub_005676b0: EXE 0x005676b0, 25 bytes. Finalizer body of the class with
// vtable 0x00727e54 (deleting finalizer sub_00567790 in slot 0). Same
// intrusive-list wrapper family as sub_004491e0: polymorphic head at +0
// (vtable slot restored here), node base at +4 with links at +4/+8 and
// owning-list pointer at +0xc. When the object still owns a list, the body
// decrements the counter at list+8 and tail-jumps into the 93-byte external
// unlink method sub_00454e70 on the node base (this + 4). Binding the node
// base as a reference pins the vtable-store / owner-load / this-adjustment /
// test schedule. Sibling bodies: sub_00567670 (vtable 0x00727e3c, unlink
// sub_00454c30), sub_00567690 (0x00727e48, sub_00454c90), sub_005676d0
// (0x00727e60, sub_00454e10). Compiled with MSVC 7.1 (/O2 /Zl).
// Raw-address vtable store: no object-local vtable is emitted.

extern "C" const void* const gVtable00727e54[]; // EXE .rdata 0x00727e54

class Head005676b0 {
public:
    unsigned int field_00;   // vtable slot; 0x00727e54 for this class
};

class Node005676b0 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_00454e70();   // external unlink method, EXE 0x00454e70
};

class Object005676b0 : public Head005676b0, public Node005676b0 {
public:
    void sub_005676b0();
};

void Object005676b0::sub_005676b0() {
    Head005676b0::field_00 = (unsigned int)gVtable00727e54;
    Node005676b0 &node = *static_cast<Node005676b0 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_00454e70();
    }
}
