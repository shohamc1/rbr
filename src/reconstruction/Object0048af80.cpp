// Object0048af80: an object with two float fields.
//
// Merge evidence: in functions 0x0053d7f0 and 0x0053d9d0 one `mov ecx,esi`
// feeds a conditional branch whose two arms call sub_0048af80 (0x0053d859,
// 0x0053da27) or sub_0048af90 (0x0053d860, 0x0053da2e) on the same object.
// Names are hypotheses.
struct Object0048af80 {
    unsigned char unknown_00[0x80];
    float field_80;
    float field_84;

    float sub_0048af80();
    float sub_0048af90();
};
static_assert(__builtin_offsetof(Object0048af80, field_80) == 0x80, "field offset");
static_assert(__builtin_offsetof(Object0048af80, field_84) == 0x84, "field offset");

float Object0048af80::sub_0048af80() { return field_80; }
float Object0048af80::sub_0048af90() { return field_84; }
