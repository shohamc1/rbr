// sub_00412040: MSVC 7.1 (/O2 /Zl).
//
// thiscall list member with one stack argument, RET 4. The receiver is a
// queue record (head at +0, tail at +4) of nodes with prev at +4, next at
// +8, and a flag byte at +0x35. Detaches the head node, repoints the next
// node's prev back at the queue unless its +0x35 flag is set, then appends
// the node at the tail of the queue (node->prev = tail, node->next = this,
// this->tail = node). The argument is an owner record whose +4 list is
// fixed up first: if that list's tail is this queue, its tail becomes the
// node; otherwise the queue's own tail link is repaired through +8 or +0.
#include "layout.h"

class Object00412040 {
public:
    Object00412040* field_00;
    Object00412040* field_04;
    Object00412040* field_08;
    unsigned char unknown_0c[0x29];
    unsigned char field_35;

    void sub_00412040(Object00412040* owner);
};

LAYOUT_OFFSET(Object00412040, field_04, 0x04);
LAYOUT_OFFSET(Object00412040, field_08, 0x08);
LAYOUT_OFFSET(Object00412040, field_35, 0x35);

// FUNCTION: RBR 0x00412040
void Object00412040::sub_00412040(Object00412040* owner) {
    Object00412040* node = field_00;
    field_00 = node->field_08;
    if (!node->field_08->field_35)
        node->field_08->field_04 = this;
    node->field_04 = field_04;
    Object00412040* list = owner->field_04;
    if (this == list->field_04)
        list->field_04 = node;
    else if (this == field_04->field_08)
        field_04->field_08 = node;
    else
        field_04->field_00 = node;
    node->field_08 = this;
    field_04 = node;
}
