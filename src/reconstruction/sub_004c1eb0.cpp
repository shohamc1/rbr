// sub_004c1eb0: EXE 0x004c1eb0, 53 bytes. __cdecl, three pointer arguments,
// return value in EAX (the advanced destination cursor).
//
// Forward 0x124-byte record copy of the 0x004c1xxx container helper family
// (caller sub_004c2bc0, four call sites 0x004c2d06-0x004c2e67). While the
// source cursor has not reached the end cursor, a non-null destination
// receives the whole 0x124-byte record (MSVC memcpy intrinsic: ECX = 0x49
// dwords, ESI = source, EDI = destination, REP MOVSD). Both cursors then
// advance one record.
struct Record004c1eb0 { unsigned char bytes[0x124]; };

extern "C" Record004c1eb0 * __cdecl sub_004c1eb0(Record004c1eb0 *begin,
                                                 Record004c1eb0 *end,
                                                 Record004c1eb0 *destination) {
    while (begin != end) {
        if (destination != 0)
            *destination = *begin;
        ++begin;
        ++destination;
    }
    return destination;
}
