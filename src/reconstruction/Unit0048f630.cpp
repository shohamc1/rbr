// sub_0048f630: Finalizer body of an intrusive-list
// wrapper with the same shape as sub_0048f5c0: polymorphic head at +0, 3-pointer
// link node at +4 with the owning list pointer at +0xc. When the object still
// owns a list, the body decrements the counter at list+8 and tail-jumps into
// the 93-byte external unlink method sub_004544b0 on the node base (this + 4).
// sub_0048f600/sub_0048f630 share the node class Node0048f600 so both emit one
// mangled callee symbol.
// Compiled with MSVC 7.1 (/O2 /Zl).

class Head0048f630 {
public:
    unsigned int field_00;   // polymorphic head, vtable slot
};

class Node0048f600 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_004544b0();   // external unlink method, EXE 0x004544b0
};

class Object0048f630 : public Head0048f630, public Node0048f600 {
public:
    void sub_0048f630();
};

// sub_0048f650: Constructor of the class whose
// deleting finalizer sub_0048f670 sits in vtable slot 0 of 0x00712f70 (the
// same class sub_0048f5e0 finalizes). ECX in, EAX out (MOV EAX,ECX prologue).
// The layout matches the finalizer family: table pointer at +0 (raw-address
// store 0x00712f70; no object-local vtable is emitted), then three dword/word
// fields cleared in the observed order +8, +4, +0xc through one zeroed
// register. No EXE reference reaches this constructor directly.
// Compiled with MSVC 7.1 (/O2 /Zl).

extern "C" const void* const gVtable00712f70[]; // EXE .rdata 0x00712f70

class Object0048f650 {
public:
    unsigned int field_00;   // table slot; 0x00712f70 stored here
    void *field_04;
    void *field_08;
    void *field_0c;

    Object0048f650();
};

// sub_0048f670: Scalar deleting finalizer (MSVC
// ??_G shape) referenced from vtable slot 0 of 0x00712f70 (slot 1 of the table
// starts the next group; slots 3, 6, and 9 hold sibling deleting finalizers).
// It runs the finalizer body sub_0048f5e0 on the same object, frees the object
// through the cdecl operator-delete thunk sub_006efdd0 when the low flag bit is
// set (add esp,4 cleanup), and returns this (EAX) with the flag argument popped
// (RET 4). Same shape as the matched sub_00449290.
// Compiled with MSVC 7.1 (/O2 /Zl).

class Object0048f5e0 {
public:
    void sub_0048f5e0();   // finalizer body, source Unit0048f5e0.cpp
};

// 6-byte cdecl operator-delete thunk at EXE 0x006efdd0 (jmp [0x0070c504]).
extern "C" void __cdecl sub_006efdd0(void *block);

class Object0048f670 {
public:
    void *sub_0048f670(unsigned int flags);
};

// FUNCTION: RBR 0x0048f630
void Object0048f630::sub_0048f630() {
    void *owner = field_08;
    Node0048f600 &node = *static_cast<Node0048f600 *>(this);
    if (owner != 0) {
        --*(int *)((char *)owner + 8);
        node.sub_004544b0();
    }
}

// FUNCTION: RBR 0x0048f650
Object0048f650::Object0048f650() {
    field_08 = 0;
    field_04 = 0;
    field_0c = 0;
    field_00 = (unsigned int)gVtable00712f70;
}

// FUNCTION: RBR 0x0048f670
void *Object0048f670::sub_0048f670(unsigned int flags) {
    ((Object0048f5e0 *)this)->sub_0048f5e0();
    if ((flags & 1) != 0) {
        sub_006efdd0(this);
    }
    return this;
}
