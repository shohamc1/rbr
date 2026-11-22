// sub_00442390: EXE 0x00442390, 42 bytes (the Ghidra packet drops the
// 3-byte `add esp,4` after the delete call; verified against raw EXE
// bytes). __thiscall member with no stack arguments, plain RET. Buffer
// release of the intrusive list/node subsystem's record container family:
// the heap block at +4 is freed through the cdecl operator-delete thunk
// sub_006efdd0 (EXE 0x006efdd0, jmp [0x0070c504], caller cleans), then the
// three slots +4/+8/+0xc are zeroed (data pointer, end pointer, capacity
// pointer of a vector-like record array). MSVC 7.1 (/O2 /Zl).
// sub_00442880 is the identical sibling body.

extern "C" void __cdecl sub_006efdd0(void* block);

class Object00442390 {
public:
    unsigned int field_00;
    void* field_04;   // heap block, freed when nonzero
    void* field_08;
    void* field_0c;

    void sub_00442390();
};

void Object00442390::sub_00442390() {
    if (field_04 != 0) {
        sub_006efdd0(field_04);
    }
    field_04 = 0;
    field_08 = 0;
    field_0c = 0;
}
