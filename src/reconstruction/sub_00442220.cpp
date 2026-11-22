// sub_00442220: EXE 0x00442220, 18 bytes. Iterator getter of the intrusive
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

void Object00442220::sub_00442220(void** out, void* unused) {
    void* node = field_00;
    sub_0043d200();
    *out = node;
}
