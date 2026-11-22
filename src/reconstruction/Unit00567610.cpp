// sub_00567610: Finalizer body of a delegate-style
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

// sub_00567640: Finalizer body of a delegate-style
// intrusive-list wrapper (the wave-2 delegate record: receiver at +0x10,
// callback at +0x14, both cleared here before the vtable restore). Same
// family as sub_004491e0: polymorphic head at +0 (vtable 0x0070e06c stored
// here), node base at +4 with links at +4/+8 and owning-list pointer at
// +0xc. When the object still owns a list, the body decrements the counter
// at list+8 and tail-jumps into the 93-byte thiscall unlink sub_0043d920 on
// the node base (this + 4), declared here through its class
// Link005ed040 (defined in Unit0043d500.cpp). Sibling sub_00567610 (vtable
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

extern "C" const void* const gVtable0070e06c[]; // EXE .rdata 0x0070e06c

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

// sub_00567670: Finalizer body of the class whose
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

extern "C" const void* const gVtable00727e3c[]; // EXE .rdata 0x00727e3c

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

// sub_00567690: Finalizer body of the class whose
// constructor is sub_00567730 and deleting finalizer sub_00567750 (vtable
// 0x00727e48, slot 0). Same intrusive-list wrapper family as sub_004491e0:
// polymorphic head at +0 (vtable slot restored here), node base at +4 with
// links at +4/+8 and owning-list pointer at +0xc. When the object still owns
// a list, the body decrements the counter at list+8 and tail-jumps into the
// 93-byte external unlink method sub_00454c90 on the node base (this + 4).
// Binding the node base as a reference pins the vtable-store / owner-load /
// this-adjustment / test schedule. Sibling bodies: sub_00567670 (vtable
// 0x00727e3c, unlink sub_00454c30), sub_005676b0 (0x00727e54, sub_00454e70),
// sub_005676d0 (0x00727e60, sub_00454e10). Compiled with MSVC 7.1 (/O2 /Zl).
// Raw-address vtable store: no object-local vtable is emitted.

extern "C" const void* const gVtable00727e48[]; // EXE .rdata 0x00727e48

class Head00567690 {
public:
    unsigned int field_00;   // vtable slot; 0x00727e48 for this class
};

class Node00567690 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_00454c90();   // external unlink method, EXE 0x00454c90
};

class Object00567690 : public Head00567690, public Node00567690 {
public:
    void sub_00567690();
};

// sub_005676b0: Finalizer body of the class with
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

// sub_005676d0: Finalizer body of the class with
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

extern "C" const void* const gVtable00727e60[]; // EXE .rdata 0x00727e60

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

// sub_005676f0: Constructor of the intrusive-list
// wrapper class whose finalizer is sub_00567670 and deleting finalizer
// sub_00567710 (vtable 0x00727e3c, slot 0): MOV EAX,ECX prologue with this
// returned in EAX. The wrapper is a polymorphic head at +0 followed by a
// 3-pointer link node at +4 (links at +4/+8, owning-list pointer at +0xc);
// the constructor zeroes the three node fields — note the observed order
// +8, +4, +0xc — then stores the raw vtable address. The vtable slot is a
// plain field (raw-address store), so no object-local vtable is emitted and
// the store order stays under source control. Compiled with MSVC 7.1 (/O2 /Zl).

extern "C" const void* const gVtable00727e3c[]; // EXE .rdata 0x00727e3c

class Object005676f0 {
public:
    unsigned int field_00;   // vtable slot; 0x00727e3c for this class
    void *field_04;          // node link
    void *field_08;          // node link
    void *field_0c;          // owning list pointer

    Object005676f0();
};

// FUNCTION: RBR 0x00567610
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

// FUNCTION: RBR 0x00567640
void Object00567640::sub_00567640() {
    void *owner;
    void *zero = 0;
    *(void * volatile *)((char *)this + 0x10) = zero;
    *(void * volatile *)((char *)this + 0x14) = zero;
    *(unsigned int volatile *)((char *)this) = (unsigned int)gVtable0070e06c;
    owner = *(void * volatile *)((char *)this + 0xc);
    Link005ed040 &node = *static_cast<Link005ed040 *>(this);
    if (owner != zero) {
        --*(int *)((char *)owner + 8);
        node.sub_0043d920();
    }
}

// FUNCTION: RBR 0x00567670
void Object00567670::sub_00567670() {
    Head00567670::field_00 = (unsigned int)gVtable00727e3c;
    Node00567670 &node = *static_cast<Node00567670 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_00454c30();
    }
}

// FUNCTION: RBR 0x00567690
void Object00567690::sub_00567690() {
    Head00567690::field_00 = (unsigned int)gVtable00727e48;
    Node00567690 &node = *static_cast<Node00567690 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_00454c90();
    }
}

// FUNCTION: RBR 0x005676b0
void Object005676b0::sub_005676b0() {
    Head005676b0::field_00 = (unsigned int)gVtable00727e54;
    Node005676b0 &node = *static_cast<Node005676b0 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_00454e70();
    }
}

// FUNCTION: RBR 0x005676d0
void Object005676d0::sub_005676d0() {
    Head005676d0::field_00 = (unsigned int)gVtable00727e60;
    Node005676d0 &node = *static_cast<Node005676d0 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_00454e10();
    }
}

// FUNCTION: RBR 0x005676f0
Object005676f0::Object005676f0() {
    field_08 = 0;
    field_04 = 0;
    field_0c = 0;
    field_00 = (unsigned int)gVtable00727e3c;
}
