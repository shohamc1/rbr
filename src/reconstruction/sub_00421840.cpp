// sub_00421840: EXE 0x00421840, 81 bytes. MSVC 7.1 (/O2 /Zl).
//
// Link step of the hierarchical intrusive list of sub_004213b0 (node links
// +0/+4/+8, sentinel flag byte +0x2d). __thiscall on the node being spliced
// (ECX) with the container record as the one stack argument; the container's
// +4 field is the list record whose +4 field is the head link. The node at
// this->field_08 is linked into the position this occupies: this->field_08
// takes that node's +0 link, a non-sentinel +0 link target's backlink (+4)
// is repointed at this, the node's backlink takes this's backlink (this->field_04),
// and the record that referenced this (list head at list->field_04, or the +0
// link of the node at this->field_04, or its +8 link) is repointed at the
// node, whose +0 link becomes this and whose backlink becomes the node.
// Called from sub_00421a70 at 0x00421c08, 0x00421c5a, 0x00421cbe and from
// sub_004213f0 at 0x004214f3; sub_004218a0 is the mirror operation with the
// +0/+8 link roles swapped.

class Object00421840 {
public:
    Object00421840* field_00;  // sibling-side link, +0
    Object00421840* field_04;  // backlink, +4
    Object00421840* field_08;  // child-side link, +8
    unsigned char field_0c[0x20];
    unsigned char field_2c;
    unsigned char flag_2d;     // sentinel flag

    void sub_00421840(Object00421840* container);
};

void Object00421840::sub_00421840(Object00421840* container) {
    Object00421840* promoted = field_08;
    field_08 = promoted->field_00;
    if (promoted->field_00->flag_2d == 0) {
        promoted->field_00->field_04 = this;
    }
    promoted->field_04 = field_04;
    Object00421840* list = container->field_04;
    if (this == list->field_04) {
        list->field_04 = promoted;
        promoted->field_00 = this;
        field_04 = promoted;
    } else if (this == field_04->field_00) {
        field_04->field_00 = promoted;
        promoted->field_00 = this;
        field_04 = promoted;
    } else {
        field_04->field_08 = promoted;
        promoted->field_00 = this;
        field_04 = promoted;
    }
}
