// sub_00498010: MSVC 7.1 (/O2 /Zl).
//
// Iterator advance (operator++) of the hierarchical allocator subsystem's
// outer node tree (same node type as the release walker sub_00498ea0:
// next sibling +0, link +4, first child +8, terminator flag byte +0x1d;
// sibling lists end at a sentinel node whose flag byte is set). __thiscall
// mutator of a one-word iterator record holding the current node; when the
// current node is already the sentinel it does nothing. Otherwise it
// descends: if the current node has a child, it walks the child's sibling
// chain (+0) to its last non-sentinel node and stores that; otherwise it
// climbs through the +4 links, staying on a node while the iterator's
// current node equals that node's +8 word, and stores the first candidate
// that fails either test. Both rotated loops run 16-byte aligned loop heads
// (LEA nop padding in the reference).

class Node00498010 {
public:
    Node00498010* field_00;   // next sibling (sentinel-terminated)
    Node00498010* field_04;   // climb link
    Node00498010* field_08;   // first child
    unsigned char unknown_0c[0x11];
    unsigned char field_1d;   // terminator flag byte (+0x1d)
};

class Iterator00498010 {
public:
    Node00498010* node;
    void sub_00498010();
};

// sub_00498ea0: (Ghidra packet drops the loop tail
// after the delete call; verified against raw EXE bytes). MSVC 7.1 (/O2 /Zl).
//
// Recursive release walk of the outer node list of the hierarchical
// allocator subsystem (see sub_00499180/sub_00499270). __thiscall member of
// the outer container (sentinel node at +4, count at +8) with the first node
// passed as a stack argument (RET 4). Each node links its next sibling at
// +0, its first child at +8, an embedded inner container at +0x10 (released
// by the callee below), and the list-terminator flag byte at +0x1d; sibling
// lists end at a sentinel node whose flag byte is set. Per node: releases
// the child subtree recursively, remembers the next sibling, releases the
// node's embedded inner container through sub_00498730 (EXE 0x00498730:
// walks the container at node+0x10 through sub_00444200 and deletes its
// sentinel through sub_006efdd0), frees the node through the cdecl
// operator-delete thunk sub_006efdd0 (add esp,4 cleanup), and continues with
// the next sibling while its flag is clear.

// 6-byte cdecl operator-delete thunk at EXE 0x006efdd0 (jmp [0x0070c504]).
extern "C" void __cdecl sub_006efdd0(void *block);

class Node00498ea0 {
public:
    Node00498ea0* field_00;   // next sibling (sentinel-terminated)
    void* field_04;
    Node00498ea0* field_08;   // first child
    unsigned char unknown_0c[0x11];
    unsigned char field_1d;   // terminator flag byte (+0x1d)

    void sub_00498730();   // inner-container release, EXE 0x00498730
};

class Object00498ea0 {
public:
    unsigned int field_00;
    void* field_04;         // sentinel node
    unsigned int field_08;  // count

    void sub_00498ea0(void* node);
};

// FUNCTION: RBR 0x00498010
void Iterator00498010::sub_00498010() {
    Node00498010* current = node;
    if (current->field_1d != 0) {
        return;
    }
    Node00498010* child = current->field_08;
    if (child->field_1d == 0) {
        while (child->field_00->field_1d == 0) {
            child = child->field_00;
        }
        node = child;
        return;
    }
    Node00498010* candidate = current->field_04;
    while (candidate->field_1d == 0 && node == candidate->field_08) {
        node = candidate;
        candidate = candidate->field_04;
    }
    node = candidate;
}

// FUNCTION: RBR 0x00498ea0
void Object00498ea0::sub_00498ea0(void* node) {
    Node00498ea0* n = (Node00498ea0*)node;
    while (((Node00498ea0*)node)->field_1d == 0) {
        sub_00498ea0(n->field_08);
        n = n->field_00;
        ((Node00498ea0*)node)->sub_00498730();
        sub_006efdd0(node);
        node = n;
    }
}
