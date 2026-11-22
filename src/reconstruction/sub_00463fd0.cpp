// sub_00463fd0: EXE 0x00463fd0, 17 bytes. __thiscall address getter taking
// two integers, RET 8.
//
// Returns &field_00[first + second*4]: the address of a float at offset 0 of
// the object, indexed column-major (the second index selects a group of four
// floats). Companion of the value getter sub_00463fb0 (0x00463fb0); the
// concrete element count is not bounded by the body.

class Object00463fd0 {
public:
    float field_00[16];

    float* sub_00463fd0(int first, int second);
};

float* Object00463fd0::sub_00463fd0(int first, int second) {
    return &field_00[first + second * 4];
}
