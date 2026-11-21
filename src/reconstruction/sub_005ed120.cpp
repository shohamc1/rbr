// sub_005ed120: EXE 0x005ed120, 8 bytes. __thiscall member, no parameters.
//
// Delegate invoker for the callback record built by sub_005ed0f0: tail-jumps
// to the function pointer at +0x14 with the receiver from +0x10 in ECX
// (one-argument __fastcall shape). `this` is cached in EAX across the ECX
// reload and is live at the jump.

class Object005ed120 {
public:
    void *field_00;
    void *field_04;
    void *field_08;
    void *field_0c;
    void *field_10;
    void *field_14;

    void sub_005ed120();
};

void Object005ed120::sub_005ed120() {
    ((void (__fastcall *)(void *))field_14)(field_10);
}
