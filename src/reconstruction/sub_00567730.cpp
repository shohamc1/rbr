// sub_00567730: EXE 0x00567730, 20 bytes. Constructor of the intrusive-list
// wrapper class whose finalizer is sub_00567690 and deleting finalizer
// sub_00567750 (vtable 0x00727e48, slot 0): MOV EAX,ECX prologue with this
// returned in EAX. The wrapper is a polymorphic head at +0 followed by a
// 3-pointer link node at +4 (links at +4/+8, owning-list pointer at +0xc);
// the constructor zeroes the three node fields — note the observed order
// +8, +4, +0xc — then stores the raw vtable address. Byte-identical to
// sub_005676f0 except the vtable constant. The vtable slot is a plain field
// (raw-address store), so no object-local vtable is emitted and the store
// order stays under source control. Compiled with MSVC 7.1 (/O2 /Zl).

class Object00567730 {
public:
    unsigned int field_00;   // vtable slot; 0x00727e48 for this class
    void *field_04;          // node link
    void *field_08;          // node link
    void *field_0c;          // owning list pointer

    Object00567730();
};

Object00567730::Object00567730() {
    field_08 = 0;
    field_04 = 0;
    field_0c = 0;
    field_00 = 0x727e48;
}
