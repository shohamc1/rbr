// sub_00449220: EXE 0x00449220, 14 bytes. Clears the three pointer fields at
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

void *Object00449220::sub_00449220() {
    field_04 = 0;
    field_08 = 0;
    field_0c = 0;
    return this;
}
