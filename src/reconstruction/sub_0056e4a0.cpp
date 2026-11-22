// sub_0056e4a0: MSVC 7.1 (/O2 /Zl).
//
// Quickrace session-state handler, thiscall member with no stack arguments
// (bare RET), the tail target of the cdecl event thunk sub_005524f0, which
// passes the object at *(0x007b17cc). Also called at 0x005524d3 and
// 0x005592ff.
//
// Reads the session state through virtual slot 30 (0x78/4) of the object at
// *(0x007a1184) (the instance at 0x007ca090) and switches on it:
//   3  - nothing;
//   4/5 - thiscall sub_0056d380(this, 0x0056c450, 0): the second argument
//         is the 1-byte no-op callback at 0x0056c450 (a bare RET);
//   default - when the global at 0x01640038 is nonzero, thiscall
//         sub_0057b2e0(*(0x007b1c7c), 0xb, 0x0056da90, 0) on the message
//         receiver at 0x007b1c84; 0x0056da90 is a callback in the same
//         subsystem (it reads 0x007a1184 and 0x01640038+0x14c).
//
// Encoding lever: cases 4 and 5 are written as separate clauses with
// duplicated bodies. That gives the SUB 3/DEC/DEC JE dispatch chain with the
// two bodies tail-merged; a shared `case 4: case 5:` clause compiles to a
// CMP 3/JLE/CMP 5/JG range check instead. Unmatched states fall through to
// the trailing test (no default clause).

class Object007a1184 {
public:
    virtual void slot_00(); virtual void slot_01(); virtual void slot_02();
    virtual void slot_03(); virtual void slot_04(); virtual void slot_05();
    virtual void slot_06(); virtual void slot_07(); virtual void slot_08();
    virtual void slot_09(); virtual void slot_0a(); virtual void slot_0b();
    virtual void slot_0c(); virtual void slot_0d(); virtual void slot_0e();
    virtual void slot_0f(); virtual void slot_10(); virtual void slot_11();
    virtual void slot_12(); virtual void slot_13(); virtual void slot_14();
    virtual void slot_15(); virtual void slot_16(); virtual void slot_17();
    virtual void slot_18(); virtual void slot_19(); virtual void slot_1a();
    virtual void slot_1b(); virtual void slot_1c(); virtual void slot_1d();
    virtual int sub_slot30();  // vtable slot 30 (offset 0x78)
};

class Object007b1c7c {
public:
    void sub_0057b2e0(unsigned int kind, unsigned int callback, unsigned int flag);
};

extern "C" Object007a1184* gObject007a1184; // EXE .data 0x007a1184
extern "C" unsigned int gFlag01640038;      // EXE .data 0x01640038
extern "C" Object007b1c7c* gObject007b1c7c; // EXE .data 0x007b1c7c
extern "C" void __cdecl sub_0056c450(); // callback; its body is a bare RET
extern "C" void __cdecl sub_0056da90(); // callback

class Object0056e4a0 {
public:
    void sub_0056e4a0();
    void sub_0056d380(unsigned int callback, unsigned int flag);  // 0x0056d380
};

// FUNCTION: RBR 0x0056e4a0
void Object0056e4a0::sub_0056e4a0() {
    switch (gObject007a1184->sub_slot30()) {
    case 3:
        return;
    case 4:
        sub_0056d380((unsigned int)sub_0056c450, 0);
        return;
    case 5:
        sub_0056d380((unsigned int)sub_0056c450, 0);
        return;
    }
    if (gFlag01640038 != 0) {
        gObject007b1c7c
            ->sub_0057b2e0(0xb, (unsigned int)sub_0056da90, 0);
    }
}
