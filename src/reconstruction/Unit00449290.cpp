// sub_00449290: Scalar deleting finalizer (MSVC
// ??_G shape) referenced from vtable slot 0 of 0x0070e964. It runs the
// finalizer body sub_004491e0 on the same object, frees the object through
// the cdecl operator-delete thunk sub_006efdd0 when the low flag bit is set,
// and returns this (EAX) with the flag argument popped (RET 4).
// sub_004492b0 is the same shape for vtable 0x0070e970 with body
// sub_00449200. Compiled with MSVC 7.1 (/O2 /Zl).

class Object004491e0 {
public:
    void sub_004491e0();   // finalizer body, matched in Unit004491e0.cpp
};

// 6-byte cdecl operator-delete thunk at EXE 0x006efdd0 (jmp [0x0070c504]).
extern "C" void __cdecl sub_006efdd0(void *block);

class Object00449290 {
public:
    void *sub_00449290(unsigned int flags);
};

// sub_004492b0: Scalar deleting finalizer (MSVC
// ??_G shape) referenced from vtable slot 0 of 0x0070e970. It runs the
// finalizer body sub_00449200 on the same object, frees the object through
// the cdecl operator-delete thunk sub_006efdd0 when the low flag bit is set,
// and returns this (EAX) with the flag argument popped (RET 4).
// sub_00449290 is the same shape for vtable 0x0070e964 with body
// sub_004491e0. Compiled with MSVC 7.1 (/O2 /Zl).

class Object00449200 {
public:
    void sub_00449200();   // finalizer body, matched in Unit004491e0.cpp
};

// 6-byte cdecl operator-delete thunk at EXE 0x006efdd0 (jmp [0x0070c504]).
extern "C" void __cdecl sub_006efdd0(void *block);

class Object004492b0 {
public:
    void *sub_004492b0(unsigned int flags);
};

extern "C" const void* const gVtable0070e994[]; // EXE .rdata 0x0070e994

struct Object004495b0 {
    unsigned int vtable, field_04, field_08, field_0c, field_10, field_14;
    Object004495b0(unsigned int first, unsigned int second);
};

// FUNCTION: RBR 0x00449290
void *Object00449290::sub_00449290(unsigned int flags) {
    ((Object004491e0 *)this)->sub_004491e0();
    if ((flags & 1) != 0) {
        sub_006efdd0(this);
    }
    return this;
}

// FUNCTION: RBR 0x004492b0
void *Object004492b0::sub_004492b0(unsigned int flags) {
    ((Object00449200 *)this)->sub_00449200();
    if ((flags & 1) != 0) {
        sub_006efdd0(this);
    }
    return this;
}

// FUNCTION: RBR 0x004495b0
Object004495b0::Object004495b0(unsigned int first, unsigned int second) {
    field_08 = 0;
    field_04 = 0;
    field_0c = 0;
    vtable = (unsigned int)gVtable0070e994;
    field_10 = first;
    field_14 = second;
}
