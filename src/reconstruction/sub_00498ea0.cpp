// sub_00498ea0: EXE 0x00498ea0, 62 bytes (Ghidra packet drops the loop tail
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
