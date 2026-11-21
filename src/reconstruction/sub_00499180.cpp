// sub_00499180: EXE 0x00499180, 43 bytes. __thiscall, no stack args.
// Resets an intrusive hierarchical list: field_04 is the sentinel node and
// field_08 the count. Releases the node tree rooted at sentinel->field_04
// through sub_00498ea0 (recursive walk; sibling chain +0, child chain +8,
// terminator flag byte +0x1d), then links all three sentinel words (+0, +4,
// +8) back to the sentinel and zeroes the count. Every field_04 read is a
// fresh reload because the stores through the sentinel may alias it.

struct Node00499180 {
    Node00499180* field_00;
    void* field_04;
    void* field_08;
};

// Shared with sub_00499270.cpp; the release walker at EXE 0x00498ea0.
struct Object00498ea0 {
    unsigned int field_00;
    void* field_04;
    unsigned int field_08;
    void sub_00498ea0(void* node);
};

struct Object00499180 {
    unsigned int field_00;
    Node00499180* field_04;
    unsigned int field_08;
    void sub_00499180();
};

void Object00499180::sub_00499180()
{
    ((Object00498ea0*)this)->sub_00498ea0(field_04->field_04);
    field_04->field_04 = field_04;
    field_08 = 0;
    field_04->field_00 = field_04;
    field_04->field_08 = field_04;
}
