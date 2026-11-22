// sub_0067bc0c: __thiscall member with two stack
// words (RET 8): (node index, value). Same receiver class as sub_0067bbb2
// (Object0067bbb2), which this member calls on this at the end.
//
// Looks up the node at this+0x14[index] and its record through this+0x10
// with the node's +0x4 index, stores value into the node's +0x44, and, when
// the record's +0x20 slot array exists, links the node into the array slot
// (record->field_20)[node->field_10 + node->field_0c * 4]: a non-minus-one
// old slot value moves to the node's +0x48 and names the node (through the
// table) whose +0x4c receives this index, then the slot takes this index.
// Finally, when the node's +0x14 index is not -1, calls sub_0067bbb2 on
// this with that index and value.
//
// The re-pushed second argument from [ESP+8] at the tail call and the pops
// scheduled around the final compare come from the file-local size pragma.


class Object0067bbb2 {
public:
    struct Node {
        unsigned char unknown_00[0x4];
        unsigned int field_04;
        unsigned int field_08;
        unsigned int field_0c;
        unsigned int field_10;
        unsigned int field_14;
        unsigned char unknown_18[0x20];
        unsigned int field_38;
        unsigned char unknown_3c[0x08];
        unsigned int field_44;
        unsigned int field_48;
        unsigned int field_4c;
        unsigned int field_50;
        unsigned int field_54;
        unsigned int field_58;
    };
    struct Record {
        unsigned char unknown_00[0x20];
        int *field_20;
        unsigned char unknown_24[0x4];
        unsigned int field_28;
        unsigned int field_2c;
        unsigned int field_30;
    };
    unsigned char unknown_00[0x10];
    Record **field_10;
    Node **field_14;

    void sub_0067bbb2(unsigned int index, unsigned int value);
    void sub_0067bc0c(unsigned int index, unsigned int value);
};

#pragma optimize("s", on)
// FUNCTION: RBR 0x0067bc0c
void Object0067bbb2::sub_0067bc0c(unsigned int index, unsigned int value) {
    Node *node = field_14[index];
    Record *record = field_10[node->field_04];
    node->field_44 = value;
    int *slots = record->field_20;
    if (slots != 0) {
        int *slot = slots + node->field_10 + node->field_0c * 4;
        if (*slot != -1) {
            node->field_48 = *slot;
            field_14[*slot]->field_4c = index;
        }
        *slot = index;
    }
    if (node->field_14 != 0xffffffffu)
        sub_0067bbb2(node->field_14, value);
}
#pragma optimize("", on)
