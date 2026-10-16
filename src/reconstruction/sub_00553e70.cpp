struct Object00553e70 {
    unsigned char unknown_00[1940];
    unsigned int field_794;
    void sub_00553e70(unsigned int value);
};
static_assert(__builtin_offsetof(Object00553e70, field_794) == 1940, "field offset");
void Object00553e70::sub_00553e70(unsigned int value) { field_794 = value; }
