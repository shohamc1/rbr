// sub_0067f514: __stdcall with two pointer
// arguments (RET 8, C symbol _sub_0067f514@8).
//
// Forwards to sub_00679212: (a + 0x14, a + 0x18, *(b + 0x14), *(b + 0x18)).
// sub_00679212 (0x00679212, 256 bytes) ends in RET 0x10: it pops its four
// stack words itself, so this caller is a plain pass-through of its return
// value with no stack cleanup. The reference never loads ECX either:
// sub_00679212 is thiscall-shaped (it reads ECX as its node-table owner),
// and this function relies on its caller leaving the owner in ECX — a
// register input like sub_00411bc0's, so the C-level call leaves ECX alone.
// Callers: 0x0067f54a (sub_0067f532), 0x0068714b, 0x00687231, 0x006872d2,
// 0x00687336.
//
// The two value fields are pushed straight from memory rather than buffered
// in registers: the file-local size pragma selects the memory-push form.
//
// NONMATCHING (best draft, score 2): everything matches except the lea
// scratch for the second argument — this draft emits lea ecx,[eax+0x18];
// push ecx where the reference has lea edx,[eax+0x18]; push edx. MSVC
// 13.10.3077 picks ECX for the single call temp in every shape tried:
// pointer/unsigned argument expressions, locals in both orders, plain /O2,
// "s" and "gs" pragmas, and a thiscall-member definition (which still
// reuses ECX after this dies). Reproducing EDX likely needs a second
// surviving temp for the first argument that folds into ADD EAX,0x14.


extern "C" int __stdcall sub_00679212(unsigned int out1, unsigned int out2,
                                      unsigned int arg2, unsigned int arg3);

// sub_0067f532: __thiscall member with two stack
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

#pragma optimize("s", on)
// NONMATCHING
// FUNCTION: RBR 0x0067f514
extern "C" int __stdcall sub_0067f514(void *arg0, void *arg1) {
    unsigned int out2 = (unsigned int)arg0 + 0x18;
    unsigned int out1 = (unsigned int)arg0 + 0x14;
    return sub_00679212(out1, out2,
                        *(unsigned int *)((char *)arg1 + 0x14),
                        *(unsigned int *)((char *)arg1 + 0x18));
}
#pragma optimize("", on)

#pragma optimize("s", on)
// NONMATCHING
// FUNCTION: RBR 0x0067f532
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
#pragma optimize("", on)
