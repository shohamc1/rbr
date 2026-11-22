// sub_00504230: EXE 0x00504230, 103 bytes. MSVC 7.1 (/O2 /Zl).
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

float Object0050ab80::sub_00504230(unsigned int value, unsigned int* record) {
    float* entry = (float*)((char*)field_04 + value * 0x20 + 0x1fc);
    float* rec = (float*)record;
    float base;
    if (rec[2] != *(float*)0x00776a70) {
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
