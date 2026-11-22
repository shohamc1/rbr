// sub_0067bbb2: __thiscall member with two stack
// words (RET 8): (node index, value).
//
// Accumulates a value range along a chain in the node table at this+0x14.
// Each step replaces a non-minus-one index with the node's parent at +0x38,
// folds the value into the node's running minimum at +0x50, maximum at +0x54,
// and hit count at +0x58, and — unless the node's +0x8 index is -1 — folds
// the value into the record selected from the table at this+0x10 through the
// node's +0x4 index (minimum +0x28, maximum +0x2c, count +0x30). The walk
// then continues with the +0x8 index.
//
// sub_0067bc0c calls this member on the same object, so both belong to one
// class; the shared receiver class is Object0067bbb2.
//
// The compare-then-load interleave at the minimum update and the absence of
// loop alignment padding come from the file-local size pragma.


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
// FUNCTION: RBR 0x0067bbb2
void Object0067bbb2::sub_0067bbb2(unsigned int index, unsigned int value) {
    for (;;) {
        Node *node = field_14[index];
        if (node->field_38 != 0xffffffffu)
            node = field_14[node->field_38];
        Record *record = field_10[node->field_04];
        if (node->field_50 > value)
            node->field_50 = value;
        if (node->field_54 < value)
            node->field_54 = value;
        node->field_58 = node->field_58 + 1;
        if (node->field_08 == 0xffffffffu)
            break;
        if (record->field_28 > value)
            record->field_28 = value;
        if (record->field_2c < value)
            record->field_2c = value;
        record->field_30 = record->field_30 + 1;
        index = node->field_08;
    }
}
#pragma optimize("", on)
