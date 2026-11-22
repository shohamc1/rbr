// sub_0048f5a0: EXE 0x0048f5a0, 19 bytes. Finalizer body of an intrusive-list
// wrapper in the sub_0048cc00 logging neighborhood. The layout is a polymorphic
// head at +0 followed by a 3-pointer link node at +4: node links at +4/+8 and
// the owning list pointer at +0xc. When the object still owns a list, the body
// decrements the counter at list+8 and tail-jumps into the 93-byte thiscall
// unlink sub_0043d920 on the node base (this + 4), declared here through its
// manifest class Link005ed040 (defined in sub_0043d920.cpp). Binding the node
// base as a reference pins the owner-load/this-adjustment schedule.
// Compiled with MSVC 7.1 (/O2 /Zl).

class Head0048f5a0 {
public:
    unsigned int field_00;   // polymorphic head, vtable slot
};

class Link005ed040 {
public:
    void *field_00;   // node link, +4 in the wrapper
    void *field_04;   // node link, +8 in the wrapper
    void *field_08;   // owning list pointer, +0xc in the wrapper
    void sub_0043d920();   // external unlink method, EXE 0x0043d920
};

class Object0048f5a0 : public Head0048f5a0, public Link005ed040 {
public:
    void sub_0048f5a0();
};

void Object0048f5a0::sub_0048f5a0() {
    void *owner = field_08;
    Link005ed040 &node = *static_cast<Link005ed040 *>(this);
    if (owner != 0) {
        --*(int *)((char *)owner + 8);
        node.sub_0043d920();
    }
}
