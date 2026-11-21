// sub_004c1e40: EXE 0x004c1e40, 26 bytes. __stdcall (RET 8), two pointer
// arguments.
//
// 0x124-byte record copy of the 0x004c1xxx container helper family. When the
// destination pointer is non-null, the record is copied with the MSVC memcpy
// intrinsic: EDI receives the destination (tested for null), the source is
// loaded into ESI after the branch, ECX = 0x49 dwords, REP MOVSD.
extern "C" void * __cdecl memcpy(void *, const void *, unsigned int);
#pragma intrinsic(memcpy)

extern "C" void __stdcall sub_004c1e40(void *destination, const void *source) {
    if (destination != 0)
        memcpy(destination, source, 0x124);
}
