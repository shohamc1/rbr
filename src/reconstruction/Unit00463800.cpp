// sub_00463800: Default constructor, __thiscall,
// no parameters, returns this in EAX.
//
// Zeroes the first four byte-sized members of the object. Evidence: the
// adjacent constructor sub_00463810 (EXE 0x00463810) allocates four bytes
// with the operator-new helper sub_006efa32 and inlines this body as
// "xor ecx,ecx" plus the same four byte stores before storing the block in
// the wrapper's field_0; sub_00463840 deletes that block. This class is
// therefore the four-byte pointee of that wrapper.

class Object00463800 {
public:
    unsigned char field_00;
    unsigned char field_01;
    unsigned char field_02;
    unsigned char field_03;

    Object00463800();
};

// sub_00463840: __thiscall member, no parameters.
//
// Releases the owned block in field_0 through the cdecl operator-delete
// thunk sub_006efdd0 (EXE 0x006efdd0, jmp [0x0070c504] = IAT slot for
// ??3@YAXPAX@Z) and nulls the pointer. Callers 0x00445ce1 and 0x0044ca39
// pass the wrapper in ECX and then delete the wrapper itself through the
// same thunk. The wrapper is constructed by the adjacent sub_00463810,
// which stores a newly allocated four-byte Object00463800 in field_0.

extern "C" void __cdecl sub_006efdd0(void* memory);

class Object00463840 {
public:
    void* field_00;

    void sub_00463840();
};

// sub_00463c50: __thiscall getter, no parameters.
//
// Returns field_04 + 0x12c: a pointer 0x12c bytes past the pointer stored
// at offset 4, i.e. the address of a sub-record inside the pointee. The
// sibling getter at 0x00463c60 is identical with +0x258. Pointee identity
// and the sub-record layout are unknown.

class Object00463c50 {
public:
    unsigned char unknown_00[4];
    unsigned char* field_04;

    unsigned char* sub_00463c50();
};

struct Object00463c60 { unsigned char unknown_00[4]; unsigned int field_04; unsigned int sub_00463c60(); };
typedef char FieldOffsetCheck[sizeof(Object00463c60) == 8 ? 1 : -1];

// sub_00463fb0: __thiscall float getter taking two
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

// sub_00463fd0: __thiscall address getter taking
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

// FUNCTION: RBR 0x00463800
Object00463800::Object00463800() {
    field_00 = 0;
    field_01 = 0;
    field_02 = 0;
    field_03 = 0;
}

// FUNCTION: RBR 0x00463840
void Object00463840::sub_00463840() {
    sub_006efdd0(field_00);
    field_00 = 0;
}

// FUNCTION: RBR 0x00463c50
unsigned char* Object00463c50::sub_00463c50() {
    return field_04 + 0x12c;
}

// FUNCTION: RBR 0x00463c60
unsigned int Object00463c60::sub_00463c60() { return field_04 + 600; }

// FUNCTION: RBR 0x00463fb0
float Object00463fb0::sub_00463fb0(int first, int second) {
    return field_00[first + second * 4];
}

// FUNCTION: RBR 0x00463fd0
float* Object00463fd0::sub_00463fd0(int first, int second) {
    return &field_00[first + second * 4];
}
