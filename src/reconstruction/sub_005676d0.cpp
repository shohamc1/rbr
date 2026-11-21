// sub_005676d0: EXE 0x005676d0, 25 bytes. Finalizer body of the class with
// vtable 0x00727e60 (deleting finalizer sub_005677d0 in slot 0). Same
// intrusive-list wrapper family as sub_004491e0: polymorphic head at +0
// (vtable slot restored here), node base at +4 with links at +4/+8 and
// owning-list pointer at +0xc. When the object still owns a list, the body
// decrements the counter at list+8 and tail-jumps into the 93-byte external
// unlink method sub_00454e10 on the node base (this + 4). Binding the node
// base as a reference pins the vtable-store / owner-load / this-adjustment /
// test schedule. Sibling bodies: sub_00567670 (vtable 0x00727e3c, unlink
// sub_00454c30), sub_00567690 (0x00727e48, sub_00454c90), sub_005676b0
// (0x00727e54, sub_00454e70). Compiled with MSVC 7.1 (/O2 /Zl).
// Raw-address vtable store: no object-local vtable is emitted.

class Head005676d0 {
public:
    unsigned int field_00;   // vtable slot; 0x00727e60 for this class
};

class Node005676d0 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_00454e10();   // external unlink method, EXE 0x00454e10
};

class Object005676d0 : public Head005676d0, public Node005676d0 {
public:
    void sub_005676d0();
};

void Object005676d0::sub_005676d0() {
    Head005676d0::field_00 = 0x727e60;
    Node005676d0 &node = *static_cast<Node005676d0 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_00454e10();
    }
}
