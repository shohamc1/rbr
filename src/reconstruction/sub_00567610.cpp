// sub_00567610: EXE 0x00567610, 33 bytes. Finalizer body of a delegate-style
// intrusive-list wrapper (the wave-2 delegate record: receiver at +0x10,
// callback at +0x14, both cleared here before the vtable restore). Same
// family as sub_004491e0: polymorphic head at +0 (vtable 0x00711108 stored
// here), node base at +4 with links at +4/+8 and owning-list pointer at
// +0xc. When the object still owns a list, the body decrements the counter
// at list+8 and tail-jumps into the 93-byte external unlink method
// sub_004537f0 on the node base (this + 4). Sibling sub_00567640 (vtable
// 0x0070e06c, unlink sub_0043d920) shares the 33-byte shape and is the
// matched reference for this encoding. Compiled with MSVC 7.1 (/O2 /Zl).
// Raw-address vtable store: no object-local vtable is emitted.
//
// Encoding levers (proven on sub_00567640): the zero value must stay in EDX
// with the owner in EAX and live through the compare (CMP EAX,EDX, not TEST
// EAX,EAX). That needs a named `zero` local compared against, `owner`
// declared before it (declaration order fixes the register homes), and the
// two clears, the vtable store, and the owner read all volatile: plain /O2
// otherwise hoists the owner load above the clears (volatile on only one
// side does not stop it; /Os and optimize("g", off) wreck the stores).

extern "C" const void* const gVtable00711108[]; // EXE .rdata 0x00711108

class Head00567610 {
public:
    unsigned int field_00;   // vtable slot; 0x00711108 for this class
};

class Node00567610 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_004537f0();   // external unlink method, EXE 0x004537f0
};

class Object00567610 : public Head00567610, public Node00567610 {
public:
    void *field_10;   // receiver, wrapper +0x10
    void *field_14;   // callback, wrapper +0x14
    void sub_00567610();
};

void Object00567610::sub_00567610() {
    void *owner;
    void *zero = 0;
    *(void * volatile *)((char *)this + 0x10) = zero;
    *(void * volatile *)((char *)this + 0x14) = zero;
    *(unsigned int volatile *)((char *)this) = (unsigned int)gVtable00711108;
    owner = *(void * volatile *)((char *)this + 0xc);
    Node00567610 &node = *static_cast<Node00567610 *>(this);
    if (owner != zero) {
        --*(int *)((char *)owner + 8);
        node.sub_004537f0();
    }
}
