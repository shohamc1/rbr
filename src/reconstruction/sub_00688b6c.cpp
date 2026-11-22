// sub_00688b6c: __thiscall member, no parameters.
//
// Returns the pointer at +0x8 of the object referenced at +0x14, or null when
// +0x14 is null. Callers 0x006904e8 and 0x0068f47b pass this in ECX and use
// the result as a pointer.

class Object00688b6c {
public:
    struct Link {
        unsigned char unknown_00[0x8];
        void *field_08;
    };
    unsigned char unknown_00[0x14];
    Link *field_14;

    void *sub_00688b6c();
};

// FUNCTION: RBR 0x00688b6c
void *Object00688b6c::sub_00688b6c() {
    if (field_14 == 0)
        return 0;
    return field_14->field_08;
}
