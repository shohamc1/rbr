// sub_0048f600: EXE 0x0048f600, 33 bytes. Finalizer body of the class whose
// deleting finalizer sub_0048f810 sits in vtable slot 9 of 0x00712f70 (called
// at 0x0048f813). The layout extends the sub_0048f5e0 wrapper shape with two
// extra dword fields at +0x10/+0x14: polymorphic head at +0 (this body restores
// the base table 0x00712b34, whose own deleting finalizer 0x0048aac0 sits in
// its slot 0), 3-pointer link node at +4 with the owning list pointer at +0xc,
// then the extra pair. The body clears the extra pair through one zeroed
// register, stores the base table, and releases the owned list exactly like
// sub_0048f5e0 but through the unlink sub_004544b0 (also byte-identical to the
// thiscall unlink sub_0043d920). sub_0048f600/sub_0048f630 share the node class
// Node0048f600 for one mangled callee symbol. The zero register stays live into
// the owner test (CMP against it instead of TEST EAX,EAX).
// Compiled with MSVC 7.1 (/O2 /Zl).

class Head0048f600 {
public:
    unsigned int field_00;   // vtable slot; 0x00712b34 restored here
};

class Node0048f600 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_004544b0();   // external unlink method, EXE 0x004544b0
};

class Object0048f600 : public Head0048f600, public Node0048f600 {
public:
    unsigned int field_10;   // cleared by this finalizer
    unsigned int field_14;   // cleared by this finalizer
    void sub_0048f600();
};

void Object0048f600::sub_0048f600() {
    field_10 = 0;
    field_14 = 0;
    Head0048f600::field_00 = 0x712b34;
    Node0048f600 &node = *static_cast<Node0048f600 *>(this);
    void *owner = node.field_08;
    if (owner != 0) {
        --*(int *)((char *)owner + 8);
        node.sub_004544b0();
    }
}
