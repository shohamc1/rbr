// sub_0048f5e0: Finalizer body of the class whose
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

// sub_0048f600: Finalizer body of the class whose
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

extern "C" const void* const gVtable00712b34[]; // EXE .rdata 0x00712b34

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

// FUNCTION: RBR 0x0048f5e0
void Object0048f5e0::sub_0048f5e0() {
    Head0048f5e0::field_00 = (unsigned int)gVtable00712f70;
    Node0048f4f0 &node = *static_cast<Node0048f4f0 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_00454ab0();
    }
}

// FUNCTION: RBR 0x0048f600
void Object0048f600::sub_0048f600() {
    field_10 = 0;
    field_14 = 0;
    Head0048f600::field_00 = (unsigned int)gVtable00712b34;
    Node0048f600 &node = *static_cast<Node0048f600 *>(this);
    void *owner = node.field_08;
    if (owner != 0) {
        --*(int *)((char *)owner + 8);
        node.sub_004544b0();
    }
}
