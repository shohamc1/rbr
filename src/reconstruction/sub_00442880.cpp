// sub_00442880: EXE 0x00442880, 42 bytes (the Ghidra packet drops the
// 3-byte `add esp,4` after the delete call; verified against raw EXE
// bytes). Byte-identical to sub_00442390: __thiscall member with no stack
// arguments, plain RET. Frees the heap block at +4 through the cdecl
// operator-delete thunk sub_006efdd0 (EXE 0x006efdd0, jmp [0x0070c504],
// caller cleans), then zeroes +4/+8/+0xc of the vector-like record
// container. A separate instantiation of the same template body.
// MSVC 7.1 (/O2 /Zl).

extern "C" void __cdecl sub_006efdd0(void* block);

class Object00442880 {
public:
    unsigned int field_00;
    void* field_04;   // heap block, freed when nonzero
    void* field_08;
    void* field_0c;

    void sub_00442880();
};

void Object00442880::sub_00442880() {
    if (field_04 != 0) {
        sub_006efdd0(field_04);
    }
    field_04 = 0;
    field_08 = 0;
    field_0c = 0;
}
