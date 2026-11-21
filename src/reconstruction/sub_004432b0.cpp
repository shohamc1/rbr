// sub_004432b0: EXE 0x004432b0, 62 bytes (Ghidra packet drops the loop tail
// after the delete call; verified against raw EXE bytes). MSVC 7.1 (/O2 /Zl).
//
// Recursive release walk of the inner node list of the hierarchical
// allocator subsystem (see sub_00499270/sub_00499180). __thiscall member of
// the inner container (sentinel node at +4, count at +8) with the first node
// passed as a stack argument (RET 4). Each node links its next sibling at
// +0 and its first child at +8, and carries the list-terminator flag byte at
// +0x2d; sibling lists end at a sentinel node whose flag byte is set. Per
// node: releases the child subtree recursively, remembers the next sibling,
// destroys the node through sub_0043d6e0 (EXE 0x0043d6e0: add ecx,0xc; jmp
// [MSVCP71 basic_string dtor at IAT 0x0070c2d8] — the node embeds a
// basic_string at +0xc), frees the node through the cdecl operator-delete
// thunk sub_006efdd0 (add esp,4 cleanup), and continues with the next
// sibling while its flag is clear.

// 6-byte cdecl operator-delete thunk at EXE 0x006efdd0 (jmp [0x0070c504]).
extern "C" void __cdecl sub_006efdd0(void *block);

class Node004432b0 {
public:
    Node004432b0* field_00;   // next sibling (sentinel-terminated)
    void* field_04;
    Node004432b0* field_08;   // first child
    unsigned char unknown_0c[0x21];
    unsigned char field_2d;   // terminator flag byte (+0x2d)

    void sub_0043d6e0();   // basic_string dtor thunk, EXE 0x0043d6e0
};

class Object004432b0 {
public:
    unsigned int field_00;
    Node004432b0* field_04;   // sentinel node
    unsigned int field_08;    // count

    void sub_004432b0(void* node);
};

void Object004432b0::sub_004432b0(void* node) {
    Node004432b0* n = (Node004432b0*)node;
    while (((Node004432b0*)node)->field_2d == 0) {
        sub_004432b0(n->field_08);
        n = n->field_00;
        ((Node004432b0*)node)->sub_0043d6e0();
        sub_006efdd0(node);
        node = n;
    }
}
