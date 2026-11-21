// sub_00498010: EXE 0x00498010, 87 bytes. MSVC 7.1 (/O2 /Zl).
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
