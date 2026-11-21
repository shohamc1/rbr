// sub_00449290: EXE 0x00449290, 30 bytes. Scalar deleting finalizer (MSVC
// ??_G shape) referenced from vtable slot 0 of 0x0070e964. It runs the
// finalizer body sub_004491e0 on the same object, frees the object through
// the cdecl operator-delete thunk sub_006efdd0 when the low flag bit is set,
// and returns this (EAX) with the flag argument popped (RET 4).
// sub_004492b0 is the same shape for vtable 0x0070e970 with body
// sub_00449200. Compiled with MSVC 7.1 (/O2 /Zl).

class Object004491e0 {
public:
    void sub_004491e0();   // finalizer body, matched in sub_004491e0.cpp
};

// 6-byte cdecl operator-delete thunk at EXE 0x006efdd0 (jmp [0x0070c504]).
extern "C" void __cdecl sub_006efdd0(void *block);

class Object00449290 {
public:
    void *sub_00449290(unsigned int flags);
};

void *Object00449290::sub_00449290(unsigned int flags) {
    ((Object004491e0 *)this)->sub_004491e0();
    if ((flags & 1) != 0) {
        sub_006efdd0(this);
    }
    return this;
}
