// sub_00499270: __thiscall, no stack args.
// Clear-all for the same hierarchical list type as sub_00499180 (field_04
// sentinel, field_08 count). Walks the ring of nodes linked from
// sentinel->field_00; each node carries an embedded list of its own at +0x10.
// Per node: release that inner list's tree through sub_004432b0 (sibling +0,
// child +8, flag byte +0x2d) and reset the inner sentinel in place, then
// advance the cursor with sub_00498010 (iterator ++ on a node pointer whose
// object lives in this frame). After the ring is drained, runs the sub_00499180
// reset inline (the reference does not call it). Caller: sub_0046c9b0 at
// 0x0046cbf2.

struct Node00499270 {
    Node00499270* field_00;
    void* field_04;
    void* field_08;
};

// Shared with sub_00499180.cpp; the release walker at EXE 0x00498ea0.
struct Object00498ea0 {
    unsigned int field_00;
    void* field_04;
    unsigned int field_08;
    void sub_00498ea0(void* node);
};

struct Node004432b0 {
    void* field_00;
    void* field_04;
    void* field_08;
};

// Release walker at EXE 0x004432b0 for the inner node type (flag byte +0x2d).
struct Object004432b0 {
    unsigned int field_00;
    Node004432b0* field_04;
    unsigned int field_08;
    void sub_004432b0(void* node);
};

// Iterator ++ at EXE 0x00498010: mutates the node pointer it receives in ECX.
struct Iterator00498010 {
    void* node;
    void sub_00498010();
};

struct Object00499270 {
    unsigned int field_00;
    Node00499270* field_04;
    unsigned int field_08;
    void sub_00499270();
};

// FUNCTION: RBR 0x00499270
void Object00499270::sub_00499270()
{
    Node00499270* cursor = field_04->field_00;
    while (cursor != field_04) {
        Object004432b0* inner = (Object004432b0*)((unsigned char*)cursor + 0x10);
        inner->sub_004432b0(inner->field_04->field_04);
        inner->field_04->field_04 = inner->field_04;
        inner->field_08 = 0;
        inner->field_04->field_00 = inner->field_04;
        inner->field_04->field_08 = inner->field_04;
        ((Iterator00498010*)&cursor)->sub_00498010();
    }
    ((Object00498ea0*)this)->sub_00498ea0(field_04->field_04);
    field_04->field_04 = field_04;
    field_08 = 0;
    field_04->field_00 = field_04;
    field_04->field_08 = field_04;
}
