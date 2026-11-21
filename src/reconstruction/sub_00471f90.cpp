// sub_00471f90: EXE 0x00471f90, 46 bytes. __thiscall member, two stack
// parameters (RET 8): a destination pointer and a byte count.
//
// Writer of a cursor/serialization object: +0x04 is the current read
// position. Copies `count` bytes from the cursor to the destination with the
// MSVC 7.1 memcpy intrinsic (REP MOVSD dword run, then REP MOVSB tail), then
// advances the cursor by the byte count. The cursor is re-read from memory
// for the advance, so the copy leaves no redundant reload. Callers are the
// sub_004968e0 reader at 0x00496963 and 0x004969b8.

extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)

class Object00471f90 {
public:
    unsigned char unknown_00[0x4];
    unsigned char* field_04;

    void sub_00471f90(void* destination, unsigned int count);
};

typedef char Object00471f90_size[(sizeof(Object00471f90) == 8) ? 1 : -1];

void Object00471f90::sub_00471f90(void* destination, unsigned int count) {
    memcpy(destination, field_04, count);
    field_04 += count;
}
