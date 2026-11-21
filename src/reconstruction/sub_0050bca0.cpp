// sub_0050bca0: EXE 0x0050bca0, 15 bytes. MSVC 7.1 (/O2 /Zl).
//
// __cdecl delete-helper: when the pointer argument is non-null, calls vtable
// slot 0 (a scalar deleting destructor, flags = 1) on the object and lets the
// callee clean its own argument; otherwise returns immediately. The caller
// cleans the single cdecl argument (bare RET).

class Object0050bca0 {
public:
    virtual void sub(unsigned int flags); // vtable slot 0
};

extern "C" void __cdecl sub_0050bca0(Object0050bca0* object) {
    if (object != 0) {
        object->sub(1);
    }
}
