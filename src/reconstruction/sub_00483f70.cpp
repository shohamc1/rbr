// sub_00483f70: EXE 0x00483f70, 59 bytes. __thiscall member with one stack
// argument (RET 4).
//
// Intrusive-list push-back onto the header embedded at this+0x208: first at
// +0x208, tail at +0x20c, count at +0x210 (the wave-1 sentinel/count
// layout; the owner stored into the node is &header). Node links sit at
// +0x04 (next) and +0x08 (prev) with the owner at +0x0c; the leading four
// node bytes are untouched here. The tail loads through this+0x20c before
// the header pointer is formed in place (ADD ECX,0x208), the node links are
// stored between TEST and JZ, and MSVC 7.1 duplicates the tail/owner/INC
// epilogue into both arms. Binding the header as a local pointer is
// load-bearing: this-based stores keep the ADD below the branch. No
// incoming references are known; sub_004807c0's reviewed span ends nearby.

class Node00483f70 {
public:
    unsigned char unknown_00[4];
    Node00483f70* field_04;
    Node00483f70* field_08;
    void* field_0c;
};

struct List00483f70 {
    Node00483f70* field_00;
    Node00483f70* field_04;
    unsigned int field_08;
};

class Object00483f70 {
public:
    unsigned char unknown_00[0x208];
    List00483f70 field_208;

    void sub_00483f70(Node00483f70* node);
};

void Object00483f70::sub_00483f70(Node00483f70* node) {
    Node00483f70* tail = field_208.field_04;
    List00483f70* list = &field_208;
    node->field_08 = tail;
    node->field_04 = 0;
    if (tail != 0) {
        tail->field_04 = node;
    } else {
        list->field_00 = node;
    }
    list->field_04 = node;
    node->field_0c = list;
    ++list->field_08;
}
