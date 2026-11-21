// sub_00463fb0: EXE 0x00463fb0, 17 bytes. __thiscall float getter taking two
// integers, RET 8.
//
// Returns field_00[first + second*4]: a float at offset 0 of the object,
// indexed column-major (the second index selects a group of four floats).
// This and the companion address getter sub_00463fd0 (0x00463fd0) look into
// a 4x4-float matrix; the concrete element count is not bounded by the body.

class Object00463fb0 {
public:
    float field_00[16];

    float sub_00463fb0(int first, int second);
};

float Object00463fb0::sub_00463fb0(int first, int second) {
    return field_00[first + second * 4];
}
