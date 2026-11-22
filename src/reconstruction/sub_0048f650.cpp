// sub_0048f650: EXE 0x0048f650, 20 bytes. Constructor of the class whose
// deleting finalizer sub_0048f670 sits in vtable slot 0 of 0x00712f70 (the
// same class sub_0048f5e0 finalizes). ECX in, EAX out (MOV EAX,ECX prologue).
// The layout matches the finalizer family: table pointer at +0 (raw-address
// store 0x00712f70; no object-local vtable is emitted), then three dword/word
// fields cleared in the observed order +8, +4, +0xc through one zeroed
// register. No EXE reference reaches this constructor directly.
// Compiled with MSVC 7.1 (/O2 /Zl).

extern "C" const void* const gVtable00712f70[]; // EXE .rdata 0x00712f70

class Object0048f650 {
public:
    unsigned int field_00;   // table slot; 0x00712f70 stored here
    void *field_04;
    void *field_08;
    void *field_0c;

    Object0048f650();
};

Object0048f650::Object0048f650() {
    field_08 = 0;
    field_04 = 0;
    field_0c = 0;
    field_00 = (unsigned int)gVtable00712f70;
}
