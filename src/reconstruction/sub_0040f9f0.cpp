// Nonmatching draft for sub_0040f9f0 (99 bytes).
//
// The reference installs a C++ exception frame before the branch:
//   push -1; push 0x70617b; mov eax, fs:[0]; push eax; mov fs:[0], esp
// sets the EH state to 0 after the allocation, and restores fs:[0] and
// esp on both exits (62 frame bytes in total). EXE 0x70617b is a
// mov eax, 0x79b5f0 / jmp __CxxFrameHandler thunk, so the original frame
// is C++ EH, not SEH3. Reproducing it needs the catch funclet, the
// funcinfo data, and the __CxxFrameHandler import, which this checker
// cannot yet verify. This draft omits the frame; its 37 remaining bytes
// match the reference instruction for instruction (the two call/jump
// displacements are position-dependent).
extern "C" void* __cdecl sub_006efa32(unsigned int size);
extern "C" void* __stdcall sub_0040f420(void* block);

struct Object0040f9f0 {
    void* field_00;
    void* sub_0040f9f0();
};

void* Object0040f9f0::sub_0040f9f0() {
    void* block = sub_006efa32(0x40);
    field_00 = block != 0 ? sub_0040f420(block) : 0;
    return this;
}
