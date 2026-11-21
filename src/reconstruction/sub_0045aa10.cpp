// sub_0045aa10: EXE 0x0045aa10, 10 bytes. __thiscall member with no stack
// arguments, plain RET. Its address is stored as data at 0x0045aa26 (by the
// initializer at 0x0045aa20, alongside 0x00452490), 0x0045d8c8, 0x0046165e,
// and 0x006f1f50, so it is dispatched through a pointer. Forwards the
// field at +0xc as the single argument of the thiscall release walker
// sub_00456450 (EXE 0x00456450, RET 4: walks the intrusive node list whose
// head/count sit at +0/+8 of that walker's receiver, invoking virtual slot
// 0 with argument 1 on each released node). MSVC 7.1 (/O2 /Zl).

class Object0045aa10 {
public:
    unsigned char unknown_00[0x0c];
    void* field_0c;

    void sub_00456450(void* arg);   // release walker, EXE 0x00456450
    void sub_0045aa10();
};

void Object0045aa10::sub_0045aa10() {
    sub_00456450(field_0c);
}
