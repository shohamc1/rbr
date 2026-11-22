// sub_00567640: EXE 0x00567640, 33 bytes. Finalizer body of a delegate-style
// intrusive-list wrapper (the wave-2 delegate record: receiver at +0x10,
// callback at +0x14, both cleared here before the vtable restore). Same
// family as sub_004491e0: polymorphic head at +0 (vtable 0x0070e06c stored
// here), node base at +4 with links at +4/+8 and owning-list pointer at
// +0xc. When the object still owns a list, the body decrements the counter
// at list+8 and tail-jumps into the 93-byte thiscall unlink sub_0043d920 on
// the node base (this + 4), declared here through its manifest class
// Link005ed040 (defined in sub_0043d920.cpp). Sibling sub_00567610 (vtable
// 0x00711108, unlink sub_004537f0) shares the 33-byte shape. Compiled with
// MSVC 7.1 (/O2 /Zl). Raw-address vtable store: no object-local vtable is
// emitted.
//
// Encoding levers: the zero value must stay in EDX with the owner in EAX,
// and the zero value must stay live through the compare (CMP EAX,EDX, not
// TEST EAX,EAX). That needs a named `zero` local compared against, `owner`
// declared before it (declaration order fixes the register homes), and both
// the two clears and the owner read volatile: plain /O2 hoists the owner
// load above the clears (volatile on only one side does not stop it; /Os
// and optimize("g", off) wreck the stores).

class Head00567640 {
public:
    unsigned int field_00;   // vtable slot; 0x0070e06c for this class
};

class Link005ed040 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_0043d920();   // external unlink method, EXE 0x0043d920
};

class Object00567640 : public Head00567640, public Link005ed040 {
public:
    void *field_10;   // receiver, wrapper +0x10
    void *field_14;   // callback, wrapper +0x14
    void sub_00567640();
};

void Object00567640::sub_00567640() {
    void *owner;
    void *zero = 0;
    *(void * volatile *)((char *)this + 0x10) = zero;
    *(void * volatile *)((char *)this + 0x14) = zero;
    *(unsigned int volatile *)((char *)this) = 0x70e06c;
    owner = *(void * volatile *)((char *)this + 0xc);
    Link005ed040 &node = *static_cast<Link005ed040 *>(this);
    if (owner != zero) {
        --*(int *)((char *)owner + 8);
        node.sub_0043d920();
    }
}
