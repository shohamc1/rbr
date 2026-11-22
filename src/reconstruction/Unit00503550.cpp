// sub_00503550: MSVC 7.1 (/O2 /Zl).
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
// +0x14/+0x18 to the vtable at EXE 0x0071135c (its entries are code
// addresses, starting 0x00478970) plus zero. Each pair looks like an embedded
// polymorphic member {vptr, value}.
//
// NONMATCHING (score 2 of 19 instructions, 61/61 bytes): everything matches
// except the shared vtable constant. The reference materializes
// `mov edx,0x71135c` at its first use, after the three zero stores; every
// draft shape materializes it at the if-body fall-through / join entry,
// three stores earlier. Tried: statement orders both ways, a const local
// pinned between the store groups, an extern named global for the record
// (address load schedules identically to the literal), a file-static const
// pair struct (object-local data; checker rejects), #pragma optimize
// "s"/"t"/"g" ("s" and "g" change the whole prologue and register choice,
// "t" keeps the same two-instruction difference).

extern "C" const void* const gVtable0071e094[]; // EXE .rdata 0x0071e094

extern "C" const void* const gVtable0071dfd8[]; // EXE .rdata 0x0071dfd8
extern "C" const void* const gVtable0071135c[]; // EXE .rdata 0x0071135c

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

// sub_00504230: MSVC 7.1 (/O2 /Zl).
//
// thiscall float kernel with two cdecl stack arguments (RET 8), returns the
// x87 st(0) value. The receiver holds the entry table at field_04; the
// second argument is the caller's record (sub_0050ab80 stores the clamped
// stage value at record+0 and passes this function (value, record)). The
// kernel selects the entry at field_04 + value*0x20 + 0x1fc and blends:
// when the record's weight at +8 equals the float zero at 0x00776a70 it
// evaluates the 3-term dot product entry[2]*rec[5] + entry[1]*rec[4] +
// rec[3]*entry[0], otherwise it takes entry[3]; the selected base is then
// combined with the record's scale fields:
//   ((base + entry[7]*rec[7]) * rec[9]) + entry[5]*rec[10]
//     + entry[4]*rec[11] + entry[6]*rec[8]
// (offsets in bytes: entry step 4 from +0, record step 4 from +0).
//
// Declared with the matched caller sub_0050ab80's signature so one symbol
// resolves its two call sites.

class Object0050ab80 {
public:
    unsigned char unknown_00[4];
    void* field_04;
    void* field_08;
    float sub_0050ab80(int index, unsigned int addend);
    float sub_00504230(unsigned int value, unsigned int* record);
};

// NONMATCHING
// FUNCTION: RBR 0x00503550
void Object0050a910::sub_00503550() {
    vtable_00 = (void*)gVtable0071e094;
    if (field_04 != 0) {
        field_08 = 0;
    }
    field_44 = 0;
    field_04 = 0;
    field_50 = 0;
    field_4c = (void*)gVtable0071135c;
    field_0c = (void*)gVtable0071dfd8;
    field_34 = (void*)gVtable0071135c;
    field_38 = 0;
    field_24 = (void*)gVtable0071135c;
    field_28 = 0;
    field_14 = (void*)gVtable0071135c;
    field_18 = 0;
}

// FUNCTION: RBR 0x00504230
float Object0050ab80::sub_00504230(unsigned int value, unsigned int* record) {
    float* entry = (float*)((char*)field_04 + value * 0x20 + 0x1fc);
    float* rec = (float*)record;
    float base;
    if (rec[2] != 0.0f) {
        base = entry[3];
    } else {
        base = entry[2] * rec[5] + entry[1] * rec[4] + rec[3] * entry[0];
    }
    float acc = (base + entry[7] * rec[7]) * rec[9];
    acc += entry[5] * rec[10];
    acc += entry[4] * rec[11];
    acc += entry[6] * rec[8];
    return acc;
}
