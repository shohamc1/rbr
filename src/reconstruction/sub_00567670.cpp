// sub_00567670: EXE 0x00567670, 25 bytes. Finalizer body of the class whose
// constructor is sub_005676f0 and deleting finalizer sub_00567710 (vtable
// 0x00727e3c, slot 0). Same intrusive-list wrapper family as sub_004491e0:
// polymorphic head at +0 (vtable slot restored here), node base at +4 with
// links at +4/+8 and owning-list pointer at +0xc. When the object still owns
// a list, the body decrements the counter at list+8 and tail-jumps into the
// 93-byte external unlink method sub_00454c30 on the node base (this + 4).
// Binding the node base as a reference pins the vtable-store / owner-load /
// this-adjustment / test schedule. Sibling bodies: sub_00567690 (vtable
// 0x00727e48, unlink sub_00454c90), sub_005676b0 (0x00727e54, sub_00454e70),
// sub_005676d0 (0x00727e60, sub_00454e10). Compiled with MSVC 7.1 (/O2 /Zl).
// Raw-address vtable store: no object-local vtable is emitted.

class Head00567670 {
public:
    unsigned int field_00;   // vtable slot; 0x00727e3c for this class
};

class Node00567670 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_00454c30();   // external unlink method, EXE 0x00454c30
};

class Object00567670 : public Head00567670, public Node00567670 {
public:
    void sub_00567670();
};

void Object00567670::sub_00567670() {
    Head00567670::field_00 = 0x727e3c;
    Node00567670 &node = *static_cast<Node00567670 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_00454c30();
    }
}
