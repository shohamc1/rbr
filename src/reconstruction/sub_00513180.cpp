struct Object00513180 {
    unsigned char unknown_00[1408];
    unsigned int field_580;
    void sub_00513180(unsigned int value);
};
static_assert(__builtin_offsetof(Object00513180, field_580) == 1408, "field offset");
void Object00513180::sub_00513180(unsigned int value) { field_580 = value; }
