// sub_0067c2ba: __thiscall member, no parameters.
//
// Assigns each node's +0x30 word its own table index: walks the node pointer
// table at +0x14 for the unsigned count at +0x8 and stores the loop index
// into +0x30 of each node. Both +0x8 reads stay in memory (the store may
// alias). Always returns zero. Callers include 0x0067e8f1, 0x0067ed04,
// 0x00685cb8, and 0x00686978.
//
// The count bound stays a direct memory compare at both the guard and the
// loop tail with no alignment padding: the file-local size pragma selects
// that loop shape.


class Object0067c2ba {
public:
    struct Node {
        unsigned char unknown_00[0x30];
        unsigned int field_30;
    };
    unsigned char unknown_00[0x8];
    unsigned int field_08;
    unsigned char unknown_0c[0x8];
    Node **field_14;

    unsigned int sub_0067c2ba();
};

// sub_0067c65d: __thiscall with one stack word
// (RET 4): a scalar deleting-destructor shape. The vtable at EXE 0x0073c904
// references this function directly.
//
// It first tail-calls the shared teardown member sub_00678e7e on this (ECX
// is still this, so the call needs no register setup), then, when bit 0 of
// the flags word is set, frees this through sub_006efdd0 (the EXE-internal
// JMP thunk at 0x006efdd0 on the imported operator delete(void*)
// ??3@YAXPAX@Z). Returns this.
//
// The operator-delete argument is cleaned with POP ECX rather than
// ADD ESP,4: the file-local size pragma selects the pop form.


extern "C" void __cdecl sub_006efdd0(void *memory);

class Object0067c65d {
public:
    void sub_00678e7e();
    void *sub_0067c65d(unsigned int flags);
};

// sub_0067ce0d: __thiscall member with two stack
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

#pragma optimize("s", on)
// FUNCTION: RBR 0x0067c2ba
unsigned int Object0067c2ba::sub_0067c2ba() {
    for (unsigned int i = 0; i < field_08; i++)
        field_14[i]->field_30 = i;
    return 0;
}
#pragma optimize("", on)

#pragma optimize("s", on)
// FUNCTION: RBR 0x0067c65d
void *Object0067c65d::sub_0067c65d(unsigned int flags) {
    sub_00678e7e();
    if (flags & 1)
        sub_006efdd0(this);
    return this;
}
#pragma optimize("", on)

#pragma optimize("s", on)
// NONMATCHING
// FUNCTION: RBR 0x0067ce0d
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
#pragma optimize("", on)
