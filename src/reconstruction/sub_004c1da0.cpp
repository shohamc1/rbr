// sub_004c1da0: EXE 0x004c1da0, 49 bytes. __cdecl, three pointer arguments,
// return value unused by the only caller.
//
// Backward block copy for the 0x124-byte record family of the 0x004c1xxx
// container helpers. Caller sub_004c2bc0 (0x004c2e78) erases leading
// records: arg0 is the data start, arg1 the stepped-back source cursor,
// arg2 the shifted-down destination cursor. While the source cursor has not
// reached the data start, both cursors step down one record and the record
// is copied with the MSVC memcpy intrinsic (REP MOVSD, ECX = 0x49 dwords).
// Overlap behavior is machine copy order, highest record first.
extern "C" void * __cdecl memcpy(void *, const void *, unsigned int);
#pragma intrinsic(memcpy)

extern "C" void * __cdecl sub_004c1da0(void *block_start,
                                       unsigned char *source,
                                       unsigned char *destination) {
    unsigned char *dst = destination;
    while (block_start != source) {
        source -= 0x124;
        dst -= 0x124;
        memcpy(dst, source, 0x124);
    }
    return dst;
}
