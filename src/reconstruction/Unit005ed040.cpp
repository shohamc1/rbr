// sub_005ed040: __thiscall member, no parameters.
//
// Callback-record unlink wrapper (record built by sub_005ed0f0: dispatch
// table at +0, link node at +4). When the link at +0xc still points at its
// owner list, decrements that list's counter at +8 and tail-jumps to the
// node-unlink helper sub_0043d920 (EXE 0x0043d920, 93 bytes) with the
// embedded link node at +4 as `this` (ADD ECX,4).

struct List005ed040 {
    void *field_00;
    void *field_04;
    unsigned int field_08;
};

class Link005ed040 {
public:
    void *field_00;
    void *field_04;
    List005ed040 *field_08;

    void sub_0043d920();
};

class Object005ed040 {
public:
    void *field_00;
    Link005ed040 field_04;

    void sub_005ed040();
};

// sub_005ed0f0: Constructor, two pointer
// parameters (RET 8), ECX-in/EAX-out.
//
// Callback/delegate record: dispatch table at +0 (raw store of the EXE table
// 0x007309c8, whose first slot is the release method at 0x005ed130), an
// embedded three-pointer link node at +4/+8/+0xc (zeroed here; unlinked by
// sub_0043d920 through the wrapper sub_005ed040), the callback receiver at
// +0x10 (passed as ECX), and the callback function at +0x14 (invoked by
// sub_005ed120).

extern "C" const void* const gVtable007309c8[]; // EXE .rdata 0x007309c8

class Object005ed0f0 {
public:
    void *field_00;
    void *field_04;
    void *field_08;
    void *field_0c;
    void *field_10;
    void *field_14;

    Object005ed0f0(void *callback_object, void *callback_function);
};

// sub_005ed120: __thiscall member, no parameters.
//
// Delegate invoker for the callback record built by sub_005ed0f0: tail-jumps
// to the function pointer at +0x14 with the receiver from +0x10 in ECX
// (one-argument __fastcall shape). `this` is cached in EAX across the ECX
// reload and is live at the jump.

class Object005ed120 {
public:
    void *field_00;
    void *field_04;
    void *field_08;
    void *field_0c;
    void *field_10;
    void *field_14;

    void sub_005ed120();
};

// FUNCTION: RBR 0x005ed040
void Object005ed040::sub_005ed040() {
    Link005ed040 *link = &field_04;
    if (link->field_08) {
        --link->field_08->field_08;
        link->sub_0043d920();
    }
}

// FUNCTION: RBR 0x005ed0f0
Object005ed0f0::Object005ed0f0(void *callback_object, void *callback_function) {
    field_08 = 0;
    field_04 = 0;
    field_0c = 0;
    field_00 = (void*)gVtable007309c8;
    field_10 = callback_object;
    field_14 = callback_function;
}

// FUNCTION: RBR 0x005ed120
void Object005ed120::sub_005ed120() {
    ((void (__fastcall *)(void *))field_14)(field_10);
}
