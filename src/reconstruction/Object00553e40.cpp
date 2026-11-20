// Object00553e40: one object, reached through the global at 0x0163f0f0.
//
// Merge evidence: EXE code loads the same global as `this` for sub_00553e40
// (152 call sites, for example 0x004844b6), sub_00553e60 (46, for example
// 0x005613ba) and sub_00553e70 (11, for example 0x0057bd60). In function
// 0x0057bcf0 it feeds sub_00553e60 (0x0057bd53) and then sub_00553e70
// (0x0057bd60). Names are hypotheses.
struct Object00553e40 {
    unsigned int field_0;
    unsigned char unknown_04[0x790];
    unsigned int field_794;

    unsigned int sub_00553e40();
    unsigned int sub_00553e60();
    void sub_00553e70(unsigned int value);
};
static_assert(__builtin_offsetof(Object00553e40, field_0) == 0, "field offset");
static_assert(__builtin_offsetof(Object00553e40, field_794) == 0x794, "field offset");

unsigned int Object00553e40::sub_00553e40() { return field_0; }
unsigned int Object00553e40::sub_00553e60() { return field_794; }
void Object00553e40::sub_00553e70(unsigned int value) { field_794 = value; }
