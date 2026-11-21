// sub_00442280: EXE 0x00442280, 18 bytes. Iterator getter of the intrusive
// list/node subsystem (see sub_00442220 for the family description). This
// instantiation advances through sub_0043d320 (EXE 0x0043d320, thiscall,
// 87 bytes, same shape as sub_0043d200). Captures the pre-advance node at
// +0 in a local (ESI), advances, stores the captured node through the first
// stack argument; the second stack argument is ignored. __thiscall, RET 8.
// MSVC 7.1 (/O2 /Zl).

class Object00442280 {
public:
    void* field_00;   // current node pointer, rewritten by the advance call
    void sub_0043d320();   // advance walk, EXE 0x0043d320
    void sub_00442280(void** out, void* unused);
};

void Object00442280::sub_00442280(void** out, void* unused) {
    void* node = field_00;
    sub_0043d320();
    *out = node;
}
