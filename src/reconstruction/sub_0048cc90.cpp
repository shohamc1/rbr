// sub_0048cc90: EXE 0x0048cc90, 7 bytes. Base finalizer of the sub_0044cfe0
// class family near the logging core sub_0048cc00. It installs the vtable
// 0x00712b90 into the object at hand and returns. The vtable's slot 0 holds
// the deleting finalizer sub_0048cd50; slot 1 repeats the shared stub
// 0x006eff06 that the 0x004491e0-family vtables also use. Compiled with
// MSVC 7.1 (/O2 /Zl). Raw-address store: no object-local vtable is emitted.

class Object0048cc90 {
public:
    unsigned int field_00;   // vtable slot; 0x00712b90 when constructed
    void sub_0048cc90();
};

void Object0048cc90::sub_0048cc90() {
    field_00 = 0x712b90;
}
