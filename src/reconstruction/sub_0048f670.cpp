// sub_0048f670: EXE 0x0048f670, 30 bytes. Scalar deleting finalizer (MSVC
// ??_G shape) referenced from vtable slot 0 of 0x00712f70 (slot 1 of the table
// starts the next group; slots 3, 6, and 9 hold sibling deleting finalizers).
// It runs the finalizer body sub_0048f5e0 on the same object, frees the object
// through the cdecl operator-delete thunk sub_006efdd0 when the low flag bit is
// set (add esp,4 cleanup), and returns this (EAX) with the flag argument popped
// (RET 4). Same shape as the matched sub_00449290.
// Compiled with MSVC 7.1 (/O2 /Zl).

class Object0048f5e0 {
public:
    void sub_0048f5e0();   // finalizer body, source sub_0048f5e0.cpp
};

// 6-byte cdecl operator-delete thunk at EXE 0x006efdd0 (jmp [0x0070c504]).
extern "C" void __cdecl sub_006efdd0(void *block);

class Object0048f670 {
public:
    void *sub_0048f670(unsigned int flags);
};

void *Object0048f670::sub_0048f670(unsigned int flags) {
    ((Object0048f5e0 *)this)->sub_0048f5e0();
    if ((flags & 1) != 0) {
        sub_006efdd0(this);
    }
    return this;
}
