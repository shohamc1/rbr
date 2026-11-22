// sub_005186c0: EXE 0x005186c0, 16 bytes. MSVC 7.1 (/O2 /Zl).
//
// Finalizer body of an intrusive-list node wrapper with no vtable head: the
// object IS the node (links at +0/+4, owning-list pointer at +8). When the
// object still owns a list, the body decrements the counter at list+8 and
// tail-jumps into the 93-byte external unlink method sub_004536d0 on the
// node itself. Sibling bodies (same shape, different unlink targets):
// sub_005186b0 (sub_00453310), sub_005186d0 (sub_00453730), sub_005186e0
// (sub_00516340), sub_005186f0 (sub_00453250). No EXE call sites or data
// pointers reference these five; the dispatch mechanism is unexplained.

class Node004536d0 {
public:
    void *field_00;   // node link
    void *field_04;   // node link
    void *field_08;   // owning list pointer
    void sub_004536d0();   // external unlink method, EXE 0x004536d0
};

class Object005186c0 : public Node004536d0 {
public:
    void sub_005186c0();
};

void Object005186c0::sub_005186c0() {
    if (field_08 != 0) {
        --*(int *)((char *)field_08 + 8);
        sub_004536d0();
    }
}
