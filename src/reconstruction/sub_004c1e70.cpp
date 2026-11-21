// sub_004c1e70: EXE 0x004c1e70, 63 bytes. __cdecl, three pointer arguments,
// return value in EAX (the advanced destination cursor).
//
// Forward 16-byte record copy of the 0x004c1xxx container helper family
// (caller sub_004c2940, four call sites 0x004c2a38-0x004c2b64). While the
// source cursor has not reached the end cursor, a non-null destination
// receives the whole 0x10-byte record; MSVC stages the record copy through
// ESI (source) and EDI (destination) with an EBX shuttle, the final dword
// loaded through the then-dead ESI. Both cursors then advance one record.
struct Record004c1e70 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
};

extern "C" Record004c1e70 * __cdecl sub_004c1e70(Record004c1e70 *begin,
                                                 Record004c1e70 *end,
                                                 Record004c1e70 *destination) {
    while (begin != end) {
        if (destination != 0)
            *destination = *begin;
        ++begin;
        ++destination;
    }
    return destination;
}
