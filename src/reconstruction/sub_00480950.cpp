// sub_00480950: EXE 0x00480950, 41 bytes. __thiscall member, no parameters.
//
// Release path of the file-wrapper record constructed by sub_00480930:
// closes the handle at +0x20 unless it holds the -1 sentinel, then frees the
// blocks at +0x18 and +0x14 through the cdecl operator-delete thunk at EXE
// 0x006efdd0 (jmp [0x0070c504]); the two one-argument cdecl calls share one
// ADD ESP,8 cleanup. Callers include 0x00463970, 0x004d3830, and the
// 0x006f3c10-0x006f3c38 thunks.

#include "win32.h"

extern "C" void __cdecl sub_006efdd0(void* block);

class Object00480950 {
public:
    unsigned char unknown_00[0x14];
    void* field_14;
    void* field_18;
    unsigned char unknown_1c[4];
    void* field_20;

    void sub_00480950();
};

void Object00480950::sub_00480950() {
    if (field_20 != (void*)-1) {
        CloseHandle(field_20);
    }
    sub_006efdd0(field_18);
    sub_006efdd0(field_14);
}
