// sub_00503550: EXE 0x00503550, 61 bytes. MSVC 7.1 (/O2 /Zl).
//
// Real destructor of the class whose deleting destructor is sub_0050a910
// (vtable 0x0071e094, slot 0; the matched sub_0050a910 and the dynamic
// initializers at 0x006f895c/0x006f8a3c and sub_004e25d0 call this member).
// Declared as a member of the caller's class Object0050a910 so one symbol
// resolves the matched call sites.
//
// Body: the vtable restore sinks into the compare's branch shadow, a
// conditional release at +8 when the pointer at +4 is nonzero, then a reset
// run that zeroes +0x44/+0x04/+0x50, restores +0x0c to the table 0x0071dfd8,
// and resets the pointer pairs at +0x4c and +0x34/+0x38, +0x24/+0x28,
// +0x14/+0x18 to the shared empty record at EXE 0x0071135c (its first
// dword is 0x00478970) plus zero.
//
// NONMATCHING (score 2 of 19 instructions, 61/61 bytes): everything matches
// except the shared empty-record constant. The reference materializes
// `mov edx,0x71135c` at its first use, after the three zero stores; every
// draft shape materializes it at the if-body fall-through / join entry,
// three stores earlier. Tried: statement orders both ways, a const local
// pinned between the store groups, an extern named global for the record
// (address load schedules identically to the literal), a file-static const
// pair struct (object-local data; checker rejects), #pragma optimize
// "s"/"t"/"g" ("s" and "g" change the whole prologue and register choice,
// "t" keeps the same two-instruction difference).

class Object0050a910 {
public:
    void* vtable_00;
    void* field_04;
    void* field_08;
    void* field_0c;
    unsigned char unknown_10[4];
    void* field_14;
    void* field_18;
    unsigned char unknown_1c[8];
    void* field_24;
    void* field_28;
    unsigned char unknown_2c[8];
    void* field_34;
    void* field_38;
    unsigned char unknown_3c[8];
    void* field_44;
    unsigned char unknown_48[4];
    void* field_4c;
    void* field_50;
    void sub_00503550();
};

void Object0050a910::sub_00503550() {
    vtable_00 = (void*)0x0071e094;
    if (field_04 != 0) {
        field_08 = 0;
    }
    field_44 = 0;
    field_04 = 0;
    field_50 = 0;
    field_4c = (void*)0x0071135c;
    field_0c = (void*)0x0071dfd8;
    field_34 = (void*)0x0071135c;
    field_38 = 0;
    field_24 = (void*)0x0071135c;
    field_28 = 0;
    field_14 = (void*)0x0071135c;
    field_18 = 0;
}
