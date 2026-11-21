// sub_0067f532: EXE 0x0067f532, 93 bytes. __thiscall member with two stack
// words (RET 8): (index a, index b); returns int.
//
// Selects two nodes from the table at this+0x14, records a's index in b's
// +0x30, and calls sub_0067f514(a, b). On a negative result it returns that
// result. Otherwise it clears a's +0x0 flag bits 0x0e000000, ORs in the
// larger of the two nodes' original 0x0e000000 bits, and, when a's +0x5c is
// zero, copies b's +0x5c and +0x60 into a. Returns zero on success.
// Callers: 0x00686910 (sub_0068679b), 0x00686d21 (sub_00686966).
//
// The 0x0e000000 mask constant enregistered in EBX, the memory-operand
// +0x5c compare, and the flag-block register allocation come from the
// file-local size pragma with the mask lines ordered a_depth, depth and the
// compound &= placed after both mask reads.
//
// NONMATCHING (best draft, score 6): everything from the CALL 0x0067f514
// onward is byte-identical. The pre-call block mirrors the reference: this
// draft materializes both indices at the top (edx = [ESP+8], ecx = [ESP+4])
// and keeps index_b in EDX; the reference materializes index_a early into
// EDX (mov edx,[ESP+4]) and loads index_b late from [ESP+0x10] directly
// into EDI between PUSH EDI and the second lookup. Tried without effect:
// lookup order, table/index locals, parameter copy, inlined call argument,
// unsigned call declaration, and plain /O2.

#pragma optimize("s", on)

extern "C" int __stdcall sub_0067f514(void *node_a, void *node_b);

class Object0067f532 {
public:
    struct Node {
        unsigned int field_00;
        unsigned char unknown_04[0x2c];
        unsigned int field_30;
        unsigned char unknown_34[0x28];
        unsigned int field_5c;
        unsigned int field_60;
    };
    unsigned char unknown_00[0x14];
    Node **field_14;

    int sub_0067f532(unsigned int index_a, unsigned int index_b);
};

int Object0067f532::sub_0067f532(unsigned int index_a, unsigned int index_b) {
    Node *a = field_14[index_a];
    Node *b = field_14[index_b];
    b->field_30 = index_a;
    int result = sub_0067f514(a, b);
    if (result < 0)
        return result;
    unsigned int b_flags = b->field_00;
    unsigned int a_flags = a->field_00;
    unsigned int a_depth = a_flags & 0x0e000000;
    unsigned int depth = b_flags & 0x0e000000;
    a->field_00 &= 0xf1ffffff;
    if (a_depth > depth)
        depth = a_depth;
    a->field_00 |= depth;
    if (a->field_5c == 0) {
        a->field_5c = b->field_5c;
        a->field_60 = b->field_60;
    }
    return 0;
}
