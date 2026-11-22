// sub_00476270: EXE 0x00476270, 109 bytes. __thiscall with no stack words.
// Compiled with MSVC 7.1.
//
// Companion of sub_00474950 and sub_004767a0 on the object fed by the global
// at 0x007ca200. Reads slot 0 through sub_00474a10 (called on the slot array
// at +0x100 with output pointers to +0xf0, +0xf4, +0xf8 and +0xfc), then:
// flag bit 2 set copies +0xf4 to +0xec and negates the float at +0xfc by
// multiplying with the -1.0 constant at 0x00776a48; otherwise +0xf0 is
// copied to +0xec. Flag bit 3 set and the pointer at +0x10 non-null calls
// sub_00475c50(0) on that pointer. Callers: 0x004767fe (relies on ECX
// surviving sub_00474950), 0x0056fba6, 0x00477d2f. Names are hypotheses.
//
// NONMATCHING. Two residuals, both terminal:
// 1. Register homing slot (score 2): every instruction and encoding matches
//    except that this draft schedules MOV ESI,ECX between PUSH ESI and
//    PUSH EDI; the reference emits it after PUSH EDI. Tried without effect:
//    statement orders, *= and explicit product forms, const-first and
//    field-first operand orders, a product local, an extern global for the
//    -1.0 constant, unary negation (FCHS), a fastcall definition, argument
//    and receiver pointer locals, an explicit self local, the size and speed
//    pragmas, and /G5 /G6 /G7 /GA /Gr /Op /Oi- /O1 /Os /GL /Og-.
// 2. Calling convention: the manifest keeps the cdecl symbol _sub_00476270
//    because 0x004767fe calls this function without any ECX setup (it relies
//    on sub_00474950 preserving ECX) — the original received its receiver in
//    ECX through a convention plain C++ cannot declare for a definition.
//    This file keeps the thiscall member form (which reads ECX correctly and
//    is byte-exact but for the homing slot); its mangled symbol no longer
//    matches the manifest entry. The volatile read below and the float-first
//    statement order are load-bearing for the FLD/FMUL operand order.
#include "layout.h"

// -1.0 float shared by the reference build at 0x00776a48.

// Slot-array method at 0x00474a10 (manifest source null): reads the slot at
// the receiver and stores its fields through the four output pointers.
class Object00474a10 {
public:
    void sub_00474a10(void* out0, void* out4, void* out8, void* outc);
};

// Receiver of sub_00475c50 (0x00475c50), called with one dword argument.
class Object00475c50 {
public:
    void sub_00475c50(unsigned int argument);
};

struct Object00476270 {
    unsigned char unknown_00[0x10];
    Object00475c50* field_10;       // +0x10
    unsigned char unknown_14[0xd8]; // skips the state dword at +0xe8
    float field_ec;
    float field_f0;
    float field_f4;
    unsigned int field_f8;          // +0xf8, flag bits 2 and 3 tested
    float field_fc;
    Object00474a10 field_100;       // +0x100, slot array base

    void sub_00476270();
};

LAYOUT_OFFSET(Object00476270, field_10, 0x10);
LAYOUT_OFFSET(Object00476270, field_ec, 0xec);
LAYOUT_OFFSET(Object00476270, field_f8, 0xf8);
LAYOUT_OFFSET(Object00476270, field_fc, 0xfc);
LAYOUT_OFFSET(Object00476270, field_100, 0x100);

void Object00476270::sub_00476270() {
    field_100.sub_00474a10(&field_f0, &field_f4, &field_f8, &field_fc);
    if (field_f8 & 4) {
        // The volatile read is load-bearing: it pins the FLD of +0xfc ahead of
        // the FMUL with the -1.0 constant, as in the reference. A plain read
        // makes MSVC 7.1 load the constant first and use the field as the
        // memory operand.
        field_fc = *(volatile float *)&field_fc * -1.0f;
        field_ec = field_f4;
    } else {
        field_ec = field_f0;
    }
    if ((field_f8 & 8) && field_10 != 0)
        field_10->sub_00475c50(0);
}
