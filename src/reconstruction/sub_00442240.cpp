// sub_00442240: EXE 0x00442240, 18 bytes. Iterator getter of the intrusive
// list/node subsystem (see sub_00442220 for the family description). This
// instantiation advances through sub_0043d260 (EXE 0x0043d260, thiscall,
// 87 bytes, same shape as sub_0043d200). Captures the pre-advance node at
// +0 in a local (ESI), advances, stores the captured node through the first
// stack argument; the second stack argument is ignored. __thiscall, RET 8.
// MSVC 7.1 (/O2 /Zl).

class Object00442240 {
public:
    void* field_00;   // current node pointer, rewritten by the advance call
    void sub_0043d260();   // advance walk, EXE 0x0043d260
    void sub_00442240(void** out, void* unused);
};

void Object00442240::sub_00442240(void** out, void* unused) {
    void* node = field_00;
    sub_0043d260();
    *out = node;
}
