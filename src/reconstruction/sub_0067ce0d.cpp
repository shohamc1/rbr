// sub_0067ce0d: EXE 0x0067ce0d, 90 bytes. __thiscall member with two stack
// words (RET 8): (node index, value).
//
// Walks a chain in the node table at this+0x14. Each step first replaces a
// non-minus-one index with the node's parent index at +0x38, raises the
// node's running maximum at +0x50 to value, recurses into the first-child
// index at +0x8 (passing the node's current +0x50), then follows the
// +0x14 index. The walk stops when that index is -1 or the reloaded +0x50
// is zero. A zero value returns immediately. Node fields:
//   +0x08 first child or -1, +0x14 next index or -1, +0x38 parent or -1,
//   +0x50 running maximum.

#pragma optimize("s", on)

class Object0067ce0d {
public:
    struct Node {
        unsigned char unknown_00[0x8];
        unsigned int field_08;
        unsigned char unknown_0c[0x8];
        unsigned int field_14;
        unsigned char unknown_18[0x20];
        unsigned int field_38;
        unsigned char unknown_3c[0x14];
        unsigned int field_50;
    };
    unsigned char unknown_00[0x14];
    Node **field_14;

    void sub_0067ce0d(unsigned int index, unsigned int value);
};

// NONMATCHING (best draft, score 15 of 31 instructions): the recursive call
// (push [esi+0x50]; mov ecx,edi; push eax), the loop structure, and every
// offset match under the size pragma. The reference materializes `value`
// into EAX before PUSH EDI (8b 44 24 08 / 85 c0 / 57 / 8b f9) and keeps
// value=EAX, index=ECX around the loop; this draft pushes EDI first,
// materializes value into ECX after mov edi,ecx, and cycles value=ECX,
// index=EAX. Plain /O2 produces the early value materialization and the
// reference prologue, but buffers the recursive argument
// (mov edx,[esi+0x50]; push edx; push eax) and rotates registers the other
// way. Assigning value before index in the tail reproduces the prologue and
// value=EAX, but parks index on ESI shared with the node pointer.

void Object0067ce0d::sub_0067ce0d(unsigned int index, unsigned int value) {
    if (value == 0)
        return;
    for (;;) {
        if (index != 0xffffffffu) {
            unsigned int parent = field_14[index]->field_38;
            if (parent != 0xffffffffu)
                index = parent;
        }
        Node *node = field_14[index];
        if (node->field_50 < value)
            node->field_50 = value;
        if (node->field_08 != 0xffffffffu)
            sub_0067ce0d(node->field_08, node->field_50);
        index = node->field_14;
        if (index == 0xffffffffu)
            break;
        value = node->field_50;
        if (value == 0)
            break;
    }
}
