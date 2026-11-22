// sub_004218a0: EXE 0x004218a0, 85 bytes. MSVC 7.1 (/O2 /Zl).
//
// Mirror of sub_00421840 on the same hierarchical intrusive list (node links
// +0/+4/+8, sentinel flag byte +0x2d): every +0 link of that function is a
// +8 link here and vice versa, while the backlink (+4) and the list-head
// check at container->field_04->field_04 keep their slots. __thiscall on the
// node being spliced (ECX) with the container record as the one stack
// argument. The node at this->field_00 is linked into the position this
// occupies: this->field_00 takes that node's +8 link, a non-sentinel +8
// link target's backlink is repointed at this, the node's backlink takes
// this's backlink, and the record that referenced this (list head, or the +8
// link of the node at this->field_04, or its +0 link) is repointed at the
// node, whose +8 link becomes this and whose backlink becomes the node.
// Called from sub_00421a70 at 0x00421c3c, 0x00421c72, 0x00421cda and from
// sub_004213f0 at 0x0042155f.

class Object004218a0 {
public:
    Object004218a0* field_00;  // sibling-side link, +0
    Object004218a0* field_04;  // backlink, +4
    Object004218a0* field_08;  // child-side link, +8
    unsigned char field_0c[0x20];
    unsigned char field_2c;
    unsigned char flag_2d;     // sentinel flag

    void sub_004218a0(Object004218a0* container);
};

void Object004218a0::sub_004218a0(Object004218a0* container) {
    Object004218a0* promoted = field_00;
    field_00 = promoted->field_08;
    if (promoted->field_08->flag_2d == 0) {
        promoted->field_08->field_04 = this;
    }
    promoted->field_04 = field_04;
    Object004218a0* list = container->field_04;
    if (this == list->field_04) {
        list->field_04 = promoted;
        promoted->field_08 = this;
        field_04 = promoted;
    } else if (this == field_04->field_08) {
        field_04->field_08 = promoted;
        promoted->field_08 = this;
        field_04 = promoted;
    } else {
        field_04->field_00 = promoted;
        promoted->field_08 = this;
        field_04 = promoted;
    }
}
