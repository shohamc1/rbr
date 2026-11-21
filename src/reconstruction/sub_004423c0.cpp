// sub_004423c0: EXE 0x004423c0, 29 bytes. __thiscall member with one stack
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

void* Object004423c0::sub_004423c0(void* key) {
    void* node;
    field_54.sub_004404c0(&node, &key);
    return (char*)node + 0x10;
}
