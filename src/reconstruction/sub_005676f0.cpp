// sub_005676f0: EXE 0x005676f0, 20 bytes. Constructor of the intrusive-list
// wrapper class whose finalizer is sub_00567670 and deleting finalizer
// sub_00567710 (vtable 0x00727e3c, slot 0): MOV EAX,ECX prologue with this
// returned in EAX. The wrapper is a polymorphic head at +0 followed by a
// 3-pointer link node at +4 (links at +4/+8, owning-list pointer at +0xc);
// the constructor zeroes the three node fields — note the observed order
// +8, +4, +0xc — then stores the raw vtable address. The vtable slot is a
// plain field (raw-address store), so no object-local vtable is emitted and
// the store order stays under source control. Compiled with MSVC 7.1 (/O2 /Zl).

extern "C" const void* const gVtable00727e3c[]; // EXE .rdata 0x00727e3c

class Object005676f0 {
public:
    unsigned int field_00;   // vtable slot; 0x00727e3c for this class
    void *field_04;          // node link
    void *field_08;          // node link
    void *field_0c;          // owning list pointer

    Object005676f0();
};

Object005676f0::Object005676f0() {
    field_08 = 0;
    field_04 = 0;
    field_0c = 0;
    field_00 = (unsigned int)gVtable00727e3c;
}
