// sub_005186e0: EXE 0x005186e0, 16 bytes. MSVC 7.1 (/O2 /Zl).
//
// Finalizer body of an intrusive-list node wrapper with no vtable head: the
// object IS the node (links at +0/+4, owning-list pointer at +8). When the
// object still owns a list, the body decrements the counter at list+8 and
// tail-jumps into the 93-byte external unlink method sub_00516340 on the
// node itself. Sibling bodies (same shape, different unlink targets):
// sub_005186b0 (sub_00453310), sub_005186c0 (sub_004536d0), sub_005186d0
// (sub_00453730), sub_005186f0 (sub_00453250). No EXE call sites or data
// pointers reference these five; the dispatch mechanism is unexplained.
//
// Unlink method manifest entry pending thiscall symbol (see
// build/harness/reports/cluster16.jsonl).

class Node00516340 {
public:
    void *field_00;   // node link
    void *field_04;   // node link
    void *field_08;   // owning list pointer
    void sub_00516340();   // external unlink method, EXE 0x00516340
};

class Object005186e0 : public Node00516340 {
public:
    void sub_005186e0();
};

void Object005186e0::sub_005186e0() {
    if (field_08 != 0) {
        --*(int *)((char *)field_08 + 8);
        sub_00516340();
    }
}
