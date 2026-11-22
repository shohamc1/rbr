// sub_00463840: EXE 0x00463840, 22 bytes. __thiscall member, no parameters.
//
// Releases the owned block in field_0 through the cdecl operator-delete
// thunk sub_006efdd0 (EXE 0x006efdd0, jmp [0x0070c504] = IAT slot for
// ??3@YAXPAX@Z) and nulls the pointer. Callers 0x00445ce1 and 0x0044ca39
// pass the wrapper in ECX and then delete the wrapper itself through the
// same thunk. The wrapper is constructed by the adjacent sub_00463810,
// which stores a newly allocated four-byte Object00463800 in field_0.

extern "C" void __cdecl sub_006efdd0(void* memory);

class Object00463840 {
public:
    void* field_00;

    void sub_00463840();
};

void Object00463840::sub_00463840() {
    sub_006efdd0(field_00);
    field_00 = 0;
}
