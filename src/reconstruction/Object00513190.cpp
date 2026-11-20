// Object00513190: an object with a pointer at offset 0x580 whose pointee
// holds a state word that two predicates compare. Compiled with MSVC 7.1.
//
// Merge evidence: the loop at 0x004e1b80 loads `[edi]` as `this` for
// sub_00513190 (0x004e1b86) and then, in the same iteration, for
// sub_005131b0 (0x004e1b9c). Names are hypotheses.
struct Object00513190 {
    unsigned char unknown_00[0x580];
    unsigned int* field_580;

    unsigned int sub_00513190();
    unsigned int sub_005131b0();
};

typedef char field_580_offset[(unsigned int)&(((Object00513190*)0)->field_580) == 0x580 ? 1 : -1];

unsigned int Object00513190::sub_00513190() { return *field_580 == 1; }
unsigned int Object00513190::sub_005131b0() { return *field_580 == 2; }
