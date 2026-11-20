// Object0040f9f0 members and the finalizer of its reader block, grouped as one
// module: sub_0040fa60 passes the block that sub_0040f9f0 allocates to
// sub_0040fab0 and then to the operator delete thunk sub_006efdd0.
// The class is declared in Object0040f9f0.h. Compiled with MSVC 7.1 (/O2 /Zl).
#include "Object0040f9f0.h"

extern "C" void* __cdecl sub_006efa32(unsigned int size);
extern "C" void* __stdcall sub_0040f420(void* block);
extern "C" void __stdcall sub_0040fab0(void* block);
extern "C" void __cdecl sub_006efdd0(void* block);

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
void* Object0040f9f0::sub_0040f9f0() {
    void* block = sub_006efa32(0x40);
    field_00 = block != 0 ? sub_0040f420(block) : 0;
    return this;
}

void Object0040f9f0::sub_0040fa60() {
    void* block = field_00;
    if (block != 0) {
        sub_0040fab0(block);
        sub_006efdd0(block);
    }
    field_00 = 0;
}

// Nonmatching draft for sub_0040fab0 (156 bytes at 0x0040fab0).
//
// sub_0040fab0 is the __stdcall finalizer of the ini-reader block that
// Object0040f9f0::sub_0040fa60 releases: the caller runs this finalizer and
// then sub_006efdd0 (the operator delete thunk) on the same block. The pinned
// body ends with RET 4 at 0x0040fb49. The finalizer destroys the block's
// members in reverse declaration order:
//   1. the ini value string at block+0x1c through MSVCP71 import slot
//      0x70c2d8 (CALL [__imp_...], __thiscall);
//   2. the object held at +0x14 by the container at block+0x10:
//      sub_00411bc0(&slot, [object], object), then operator delete on the
//      object, then both hub fields clear;
//   3. the same release pattern for the hub at block+0x04 (object at +0x08).
//
// The reference installs a C++ exception frame first:
//   push -1; push 0x706166; mov eax, fs:[0]; push eax; mov fs:[0], esp
// EXE 0x706166 is a mov eax, 0x79b5cc / jmp __CxxFrameHandler thunk, so this
// is C++ EH. The EH state moves to 1 around the string destructor
// (0x0040fad0) and back to 0 after it (0x0040fae0). The epilogue at
// 0x0040fb2f restores fs:[0] from the stack slot the first sub_00411bc0 call
// wrote; the reference reuses the saved-ECX slot for that and the dead
// scope-table slot for the second release. Reproducing this needs the unwind
// funclet, the funcinfo data, and checker support for the handler-thunk
// relocation, so this draft omits the frame (the same checker limitation
// recorded for sub_0040f9f0; see analysis/notes.md, Startup subsystem).
//
// Naming the destructor also makes MSVC 7.1 emit
// the local scalar-deleting-dtor closure ??_G...QAEPAXI@Z, so the object holds
// two function symbols and check-function reports that instead of byte
// differences. EH frames still need linker-aware checking.

// EXE-internal callees. sub_006efdd0 is the 6-byte operator delete thunk
// (jmp dword ptr [0x70c504], cdecl). sub_00411bc0 consumes three stack arguments
// and the container in EDI, then returns with RET 0xc. It walks node links and
// can reset the head's links to itself. This draft omits the EDI input.
extern "C" void* __stdcall sub_00411bc0(void* slot, void* firstNode, void* lastNode);

// Local container layout; the head pointer is at offset 4.
struct ReleaseHub0040fab0 {
    void* field_00;
    void* object;
    void* field_08;
};

// The reference inlines this release into the finalizer for both hubs.
__forceinline static void releaseHub(ReleaseHub0040fab0* hub) {
    void* released;
    void* firstNode = *static_cast<void**>(hub->object);
    sub_00411bc0(&released, firstNode, hub->object);
    sub_006efdd0(hub->object);
    hub->object = 0;
    hub->field_08 = 0;
}

// The 0x40-byte block from sub_006efa32; member offsets observed here.
// The eight bytes after value are untouched by this finalizer.
struct Block0040fab0 {
    void* field_00;
    ReleaseHub0040fab0 lowHub;   // +0x04
    ReleaseHub0040fab0 highHub;  // +0x10
    std::string value;             // +0x1c
};

extern "C" void __stdcall sub_0040fab0(void* block) {
    Block0040fab0* self = static_cast<Block0040fab0*>(block);
    self->value.~basic_string();
    releaseHub(&self->highHub);
    releaseHub(&self->lowHub);
}
