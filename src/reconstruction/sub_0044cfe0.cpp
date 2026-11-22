// sub_0044cfe0: EXE 0x0044cfe0, 11 bytes. Destructor prologue of a class in
// the 0x0044cf00 family: it installs this class's vtable 0x0070ec00 (slots:
// sub_0044d200, sub_0044cff0, sub_0044d070) and then tail-jumps to the base
// finalizer sub_0048cc90 on the same object, which installs vtable 0x00712b90.
// Compiled with MSVC 7.1 (/O2 /Zl). Raw-address store: no object-local
// vtable is emitted; the tail call keeps ECX (this) unchanged.

extern "C" const void* const gVtable0070ec00[]; // EXE .rdata 0x0070ec00

class Object0048cc90 {
public:
    void sub_0048cc90();   // base finalizer, matched in sub_0048cc90.cpp
};

class Object0044cfe0 {
public:
    unsigned int field_00;   // vtable slot; 0x0070ec00 for this class
    void sub_0044cfe0();
};

void Object0044cfe0::sub_0044cfe0() {
    field_00 = (unsigned int)gVtable0070ec00;
    ((Object0048cc90 *)this)->sub_0048cc90();
}
