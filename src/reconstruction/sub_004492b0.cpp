// sub_004492b0: EXE 0x004492b0, 30 bytes. Scalar deleting finalizer (MSVC
// ??_G shape) referenced from vtable slot 0 of 0x0070e970. It runs the
// finalizer body sub_00449200 on the same object, frees the object through
// the cdecl operator-delete thunk sub_006efdd0 when the low flag bit is set,
// and returns this (EAX) with the flag argument popped (RET 4).
// sub_00449290 is the same shape for vtable 0x0070e964 with body
// sub_004491e0. Compiled with MSVC 7.1 (/O2 /Zl).

class Object00449200 {
public:
    void sub_00449200();   // finalizer body, matched in sub_00449200.cpp
};

// 6-byte cdecl operator-delete thunk at EXE 0x006efdd0 (jmp [0x0070c504]).
extern "C" void __cdecl sub_006efdd0(void *block);

class Object004492b0 {
public:
    void *sub_004492b0(unsigned int flags);
};

void *Object004492b0::sub_004492b0(unsigned int flags) {
    ((Object00449200 *)this)->sub_00449200();
    if ((flags & 1) != 0) {
        sub_006efdd0(this);
    }
    return this;
}
