// sub_00442220: Iterator getter of the intrusive
// list/node subsystem that extends the batch-100 wrapper family
// (sub_004491e0-sub_004492b0, release walkers sub_004432b0/sub_00498ea0).
// The object holds the current node pointer at +0; sub_0043d200
// (EXE 0x0043d200, thiscall, 87 bytes) advances the iterator by rewriting
// that slot (tree walk: node links +0/+4/+8, terminator flag byte +0x2d,
// owner back-link at +8 of the iterator). This member captures the
// pre-advance node in a local (pinned in ESI across the call), advances,
// and stores the captured node through the first stack argument; the second
// stack argument is accepted and ignored. __thiscall, RET 8.
// MSVC 7.1 (/O2 /Zl).
//
// Identical siblings: sub_00442240 (advance sub_0043d260), sub_00442260
// (sub_0043d2c0), sub_00442280 (sub_0043d320). Wrapper cousins that return
// this after the same advance call: sub_0043eac0/sub_0043ead0 and
// sub_0043fce0/sub_0043fcf0.

class Object00442220 {
public:
    void* field_00;   // current node pointer, rewritten by the advance call
    void sub_0043d200();   // advance walk, EXE 0x0043d200
    void sub_00442220(void** out, void* unused);
};

// sub_00442240: Iterator getter of the intrusive
// list/node subsystem (see sub_00442220 for the family description). This
// instantiation advances through sub_0043d260 (EXE 0x0043d260, thiscall,
// 87 bytes, same shape as sub_0043d200). Captures the pre-advance node at
// +0 in a local (ESI), advances, stores the captured node through the first
// stack argument; the second stack argument is ignored. __thiscall, RET 8.
// MSVC 7.1 (/O2 /Zl).

class Object00442240 {
public:
    void* field_00;   // current node pointer, rewritten by the advance call
    void sub_0043d260();   // advance walk, EXE 0x0043d260
    void sub_00442240(void** out, void* unused);
};

// sub_00442260: Iterator getter of the intrusive
// list/node subsystem (see sub_00442220 for the family description). This
// instantiation advances through sub_0043d2c0 (EXE 0x0043d2c0, thiscall,
// 87 bytes, same shape as sub_0043d200). Captures the pre-advance node at
// +0 in a local (ESI), advances, stores the captured node through the first
// stack argument; the second stack argument is ignored. __thiscall, RET 8.
// MSVC 7.1 (/O2 /Zl).

class Object00442260 {
public:
    void* field_00;   // current node pointer, rewritten by the advance call
    void sub_0043d2c0();   // advance walk, EXE 0x0043d2c0
    void sub_00442260(void** out, void* unused);
};

// sub_00442280: Iterator getter of the intrusive
// list/node subsystem (see sub_00442220 for the family description). This
// instantiation advances through sub_0043d320 (EXE 0x0043d320, thiscall,
// 87 bytes, same shape as sub_0043d200). Captures the pre-advance node at
// +0 in a local (ESI), advances, stores the captured node through the first
// stack argument; the second stack argument is ignored. __thiscall, RET 8.
// MSVC 7.1 (/O2 /Zl).

class Object00442280 {
public:
    void* field_00;   // current node pointer, rewritten by the advance call
    void sub_0043d320();   // advance walk, EXE 0x0043d320
    void sub_00442280(void** out, void* unused);
};

// sub_00442340: __cdecl forwarder of the intrusive
// list/node subsystem to the record-copy helper sub_0043fe10 (EXE 0x0043fe10,
// cdecl, reads (dst, count, src); parameters beyond the third are declared
// but unused at every call site). This function passes its four stack
// arguments straight through as (dst, count, src, fourth) and an
// indeterminate 4-byte local as the fifth argument: the PUSH ECX prologue
// slot is MSVC 7.1's indeterminate-local idiom, read back and pushed as the
// helper's fifth argument. Plain RET confirms __cdecl with four arguments;
// ECX is never read (unlike sibling sub_00443360, which forwards its
// thiscall ECX there and cleans with RET 0xc). No callers reference this
// body anywhere in the EXE. MSVC 7.1 (/O2 /Zl).

extern "C" void __cdecl sub_0043fe10(void* dst, int count, void* src,
                                     void* fourth, void* fifth);

// sub_00442370: __thiscall member (RET 0xc, three
// stack arguments) of the intrusive list/node subsystem, forwarding to the
// pair-copy helper sub_0043f350 (EXE 0x0043f350, cdecl, reads
// (begin, end, out) over 8-byte records; parameters beyond the third are
// declared but unused at every call site). The member passes
// (begin, end, out, this, out): ECX (this) and the third argument are pushed
// as the helper's trailing arguments. The third parameter is
// volatile-qualified: the two argument reads stay separate loads (no CSE),
// which also pins the whole push-run schedule — EAX/EDX carry out/end above
// the run and ECX reloads out for the third argument. Sibling shapes:
// sub_00443510 and sub_00443360 forward ECX and a repeated argument the
// same way. MSVC 7.1 (/O2 /Zl).

extern "C" void __cdecl sub_0043f350(void* begin, void* end, void* out,
                                     void* fourth, void* fifth);

class Object00442370 {
public:
    void sub_00442370(void* begin, void* end, void* volatile out);
};

// sub_00442390: (the Ghidra packet drops the
// 3-byte `add esp,4` after the delete call; verified against raw EXE
// bytes). __thiscall member with no stack arguments, plain RET. Buffer
// release of the intrusive list/node subsystem's record container family:
// the heap block at +4 is freed through the cdecl operator-delete thunk
// sub_006efdd0 (EXE 0x006efdd0, jmp [0x0070c504], caller cleans), then the
// three slots +4/+8/+0xc are zeroed (data pointer, end pointer, capacity
// pointer of a vector-like record array). MSVC 7.1 (/O2 /Zl).
// sub_00442880 is the identical sibling body.

extern "C" void __cdecl sub_006efdd0(void* block);

class Object00442390 {
public:
    unsigned int field_00;
    void* field_04;   // heap block, freed when nonzero
    void* field_08;
    void* field_0c;

    void sub_00442390();
};

// sub_004423c0: __thiscall member with one stack
// argument (key, passed by value; its address is taken), RET 4. Tree lookup
// of the intrusive list/node subsystem: the map object embedded at +0x54 is
// searched through sub_004404c0 (EXE 0x004404c0, thiscall, RET 8: writes
// the found node through its first stack argument, reads the key through
// its second), and the node pointer is returned biased by +0x10 — the
// caller-visible value subobject of the node (key at +0xc). The found node
// lives in a local slot allocated by the PUSH ECX prologue idiom.
// MSVC 7.1 (/O2 /Zl).

class Tree004423c0 {
public:
    void sub_004404c0(void** out, void* key);   // EXE 0x004404c0
};

class Object004423c0 {
public:
    unsigned char unknown_00[0x54];
    Tree004423c0 field_54;

    void* sub_004423c0(void* key);
};

// sub_00442880: (the Ghidra packet drops the
// 3-byte `add esp,4` after the delete call; verified against raw EXE
// bytes). Byte-identical to sub_00442390: __thiscall member with no stack
// arguments, plain RET. Frees the heap block at +4 through the cdecl
// operator-delete thunk sub_006efdd0 (EXE 0x006efdd0, jmp [0x0070c504],
// caller cleans), then zeroes +4/+8/+0xc of the vector-like record
// container. A separate instantiation of the same template body.
// MSVC 7.1 (/O2 /Zl).

extern "C" void __cdecl sub_006efdd0(void* block);

class Object00442880 {
public:
    unsigned int field_00;
    void* field_04;   // heap block, freed when nonzero
    void* field_08;
    void* field_0c;

    void sub_00442880();
};

// sub_004432b0: (Ghidra packet drops the loop tail
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

// FUNCTION: RBR 0x00442220
void Object00442220::sub_00442220(void** out, void* unused) {
    void* node = field_00;
    sub_0043d200();
    *out = node;
}

// FUNCTION: RBR 0x00442240
void Object00442240::sub_00442240(void** out, void* unused) {
    void* node = field_00;
    sub_0043d260();
    *out = node;
}

// FUNCTION: RBR 0x00442260
void Object00442260::sub_00442260(void** out, void* unused) {
    void* node = field_00;
    sub_0043d2c0();
    *out = node;
}

// FUNCTION: RBR 0x00442280
void Object00442280::sub_00442280(void** out, void* unused) {
    void* node = field_00;
    sub_0043d320();
    *out = node;
}

// FUNCTION: RBR 0x00442340
extern "C" void __cdecl sub_00442340(void* dst, int count, void* src,
                                     void* fourth) {
    void* fifth;
    sub_0043fe10(dst, count, src, fourth, fifth);
}

// FUNCTION: RBR 0x00442370
void Object00442370::sub_00442370(void* begin, void* end, void* volatile out) {
    sub_0043f350(begin, end, out, this, out);
}

// FUNCTION: RBR 0x00442390
void Object00442390::sub_00442390() {
    if (field_04 != 0) {
        sub_006efdd0(field_04);
    }
    field_04 = 0;
    field_08 = 0;
    field_0c = 0;
}

// FUNCTION: RBR 0x004423c0
void* Object004423c0::sub_004423c0(void* key) {
    void* node;
    field_54.sub_004404c0(&node, &key);
    return (char*)node + 0x10;
}

// FUNCTION: RBR 0x00442880
void Object00442880::sub_00442880() {
    if (field_04 != 0) {
        sub_006efdd0(field_04);
    }
    field_04 = 0;
    field_08 = 0;
    field_0c = 0;
}

// FUNCTION: RBR 0x004432b0
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
