// sub_004491e0: Finalizer body of the class whose
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

extern "C" const void* const gVtable0070e964[]; // EXE .rdata 0x0070e964

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

// sub_00449200: Finalizer body of the class whose
// deleting finalizer sub_004492b0 sits in vtable slot 0 of 0x0070e970. The
// layout is a polymorphic head at +0 (vtable slot restored here) followed by
// an intrusive-list node base at +4: node links at +4/+8 and the owning list
// pointer at +0xc. When the object still owns a list, the body decrements the
// counter at list+8 and tail-jumps into the 93-byte external unlink method
// sub_00447850 on the node base (this + 4). Binding the node base as a
// reference before the test pins the compiler's schedule: vtable store, owner
// load, this-adjustment, test. Sibling bodies: sub_004491e0 (vtable
// 0x0070e964, unlink sub_004477b0), sub_00449250 (0x0070e97c, sub_004478f0),
// sub_00449270 (0x0070e988, sub_00447710) — the four look like template
// instantiations of one wrapper. Compiled with MSVC 7.1 (/O2 /Zl).
// Raw-address vtable store: no object-local vtable is emitted.

extern "C" const void* const gVtable0070e970[]; // EXE .rdata 0x0070e970

class Head00449200 {
public:
    unsigned int field_00;   // vtable slot; 0x0070e970 for this class
};

class Node00449200 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_00447850();   // external unlink method, EXE 0x00447850
};

class Object00449200 : public Head00449200, public Node00449200 {
public:
    void sub_00449200();
};

// sub_00449220: Clears the three pointer fields at
// +4, +8, and +0xc of its object — the same offsets as the intrusive-list node
// (prev, next, owner list) that the 0x004491e0-family finalizers release —
// and returns the object pointer in EAX (constructor shape; no vtable store,
// so this is the node class itself, not the polymorphic wrapper).
// sub_00449240 is the identical body for a sibling class; the pair looks like
// template instantiations of one node/container reset. Compiled with MSVC 7.1
// (/O2 /Zl); the reference caches this in EAX and zeroes through ECX.

class Object00449220 {
public:
    unsigned int field_00;
    void *field_04;
    void *field_08;
    void *field_0c;
    void *sub_00449220();
};

// sub_00449230: Element accessor for the container
// whose base pointer sits at +4: it returns base + index * 0x24, so the
// elements are 0x24-byte records. One thiscall stack argument, cleaned by the
// callee (RET 4). Compiled with MSVC 7.1 (/O2 /Zl); the reference scales the
// index by 9 with LEA and by 4 against the base in ECX.

class Object00449230 {
public:
    unsigned int field_00;
    void *field_04;   // element array base
    void *sub_00449230(unsigned int index);
};

// sub_00449240: Clears the three pointer fields at
// +4, +8, and +0xc of its object — the same offsets as the intrusive-list node
// (prev, next, owner list) that the 0x004491e0-family finalizers release —
// and returns the object pointer in EAX (constructor shape; no vtable store,
// so this is the node class itself, not the polymorphic wrapper).
// sub_00449220 is the identical body for a sibling class; the pair looks like
// template instantiations of one node/container reset. Compiled with MSVC 7.1
// (/O2 /Zl); the reference caches this in EAX and zeroes through ECX.

class Object00449240 {
public:
    unsigned int field_00;
    void *field_04;
    void *field_08;
    void *field_0c;
    void *sub_00449240();
};

// sub_00449250: Finalizer body of the class whose
// deleting finalizer sits at 0x004492d0 in vtable slot 0 of 0x0070e97c. The
// layout is a polymorphic head at +0 (vtable slot restored here) followed by
// an intrusive-list node base at +4: node links at +4/+8 and the owning list
// pointer at +0xc. When the object still owns a list, the body decrements the
// counter at list+8 and tail-jumps into the 93-byte external unlink method
// sub_004478f0 on the node base (this + 4). Binding the node base as a
// reference before the test pins the compiler's schedule: vtable store, owner
// load, this-adjustment, test. Sibling bodies: sub_004491e0 (vtable
// 0x0070e964, unlink sub_004477b0), sub_00449200 (0x0070e970, sub_00447850),
// sub_00449270 (0x0070e988, sub_00447710) — the four look like template
// instantiations of one wrapper. Compiled with MSVC 7.1 (/O2 /Zl).
// Raw-address vtable store: no object-local vtable is emitted.

extern "C" const void* const gVtable0070e97c[]; // EXE .rdata 0x0070e97c

class Head00449250 {
public:
    unsigned int field_00;   // vtable slot; 0x0070e97c for this class
};

class Node00449250 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_004478f0();   // external unlink method, EXE 0x004478f0
};

class Object00449250 : public Head00449250, public Node00449250 {
public:
    void sub_00449250();
};

// sub_00449270: Finalizer body of the class whose
// deleting finalizer sits at 0x004492f0 in vtable slot 0 of 0x0070e988. The
// layout is a polymorphic head at +0 (vtable slot restored here) followed by
// an intrusive-list node base at +4: node links at +4/+8 and the owning list
// pointer at +0xc. When the object still owns a list, the body decrements the
// counter at list+8 and tail-jumps into the 93-byte external unlink method
// sub_00447710 on the node base (this + 4). Binding the node base as a
// reference before the test pins the compiler's schedule: vtable store, owner
// load, this-adjustment, test. Sibling bodies: sub_004491e0 (vtable
// 0x0070e964, unlink sub_004477b0), sub_00449200 (0x0070e970, sub_00447850),
// sub_00449250 (0x0070e97c, sub_004478f0) — the four look like template
// instantiations of one wrapper. Compiled with MSVC 7.1 (/O2 /Zl).
// Raw-address vtable store: no object-local vtable is emitted.

extern "C" const void* const gVtable0070e988[]; // EXE .rdata 0x0070e988

class Head00449270 {
public:
    unsigned int field_00;   // vtable slot; 0x0070e988 for this class
};

class Node00449270 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_00447710();   // external unlink method, EXE 0x00447710
};

class Object00449270 : public Head00449270, public Node00449270 {
public:
    void sub_00449270();
};

// FUNCTION: RBR 0x004491e0
void Object004491e0::sub_004491e0() {
    Head004491e0::field_00 = (unsigned int)gVtable0070e964;
    Node004491e0 &node = *static_cast<Node004491e0 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_004477b0();
    }
}

// FUNCTION: RBR 0x00449200
void Object00449200::sub_00449200() {
    Head00449200::field_00 = (unsigned int)gVtable0070e970;
    Node00449200 &node = *static_cast<Node00449200 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_00447850();
    }
}

// FUNCTION: RBR 0x00449220
void *Object00449220::sub_00449220() {
    field_04 = 0;
    field_08 = 0;
    field_0c = 0;
    return this;
}

// FUNCTION: RBR 0x00449230
void *Object00449230::sub_00449230(unsigned int index) {
    return (char *)field_04 + index * 0x24;
}

// FUNCTION: RBR 0x00449240
void *Object00449240::sub_00449240() {
    field_04 = 0;
    field_08 = 0;
    field_0c = 0;
    return this;
}

// FUNCTION: RBR 0x00449250
void Object00449250::sub_00449250() {
    Head00449250::field_00 = (unsigned int)gVtable0070e97c;
    Node00449250 &node = *static_cast<Node00449250 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_004478f0();
    }
}

// FUNCTION: RBR 0x00449270
void Object00449270::sub_00449270() {
    Head00449270::field_00 = (unsigned int)gVtable0070e988;
    Node00449270 &node = *static_cast<Node00449270 *>(this);
    if (node.field_08 != 0) {
        --*(int *)((char *)node.field_08 + 8);
        node.sub_00447710();
    }
}
