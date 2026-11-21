// sub_0049d010: EXE 0x0049d010, 40 bytes. __thiscall, one stack word (RET 4).
// Finds the first null slot among the sixteen pointers at this->field_30,
// stores the argument there, and returns 1; returns 0 when all are taken.
// The scan walks a running pointer while the one-shot store stays indexed off
// this, which is why the reference uses both EDX and [ECX+EAX*4+0x30].

struct Object0049d010 {
    unsigned char field_00[0x30];
    void* field_30[16];
    int sub_0049d010(void* value);
};

int Object0049d010::sub_0049d010(void* value)
{
    unsigned int index = 0;
    void** slot = field_30;
    do {
        if (*slot == 0) {
            field_30[index] = value;
            return 1;
        }
        index = index + 1;
        slot = slot + 1;
    } while (index < 16);
    return 0;
}
