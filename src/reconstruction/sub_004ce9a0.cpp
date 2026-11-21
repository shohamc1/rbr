// sub_004ce9a0: EXE 0x004ce9a0, 28 bytes. thiscall member with one pointer
// stack argument (RET 4).
//
// Copies a dword from the caller's record into this receiver's 0x2b0-stride
// element table: record->field_0 (loaded into EDX and scaled by IMUL) is the
// element index and record->field_0x98 (loaded into EAX) the value stored at
// this + 0xb0 + index*0x2b0. The only EXE address reference to this entry is
// the immediate at 0x004cf47a (inside a 0x004cf4xx body), not a vtable slot.
class Object004ce9a0 {
public:
    void sub_004ce9a0(void *record);
};

void Object004ce9a0::sub_004ce9a0(void *record) {
    unsigned int index = *(unsigned int *)record;
    unsigned int value = *(unsigned int *)((char *)record + 0x98);
    *(unsigned int *)((char *)this + 0xb0 + index * 0x2b0) = value;
}
