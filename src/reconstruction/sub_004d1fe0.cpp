// sub_004d1fe0: thiscall member with two stack
// arguments (RET 8): a record pointer and a float scale.
//
// Converts the record's int field at +0xc to float (FILD), multiplies by
// the scale argument (FMUL [ESP+0xC] across the saved-ESI push), truncates
// through the CRT _ftol helper at EXE 0x006efe34 (ST(0) popped into EAX),
// and stores the result at this + 0x14. Four 0x004d0xxx/0x004d03xx callers
// set ECX from a container pointer plus 0x11c and push the record before
// storing an unrelated float with FSTP [ESP].
class Object004d1fe0 {
public:
    void sub_004d1fe0(void *record, float scale);
};

// FUNCTION: RBR 0x004d1fe0
void Object004d1fe0::sub_004d1fe0(void *record, float scale) {
    *(int *)((char *)this + 0x14) =
        (int)(*(int *)((char *)record + 0xc) * scale);
}
