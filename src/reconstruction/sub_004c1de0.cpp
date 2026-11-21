// sub_004c1de0: EXE 0x004c1de0, 37 bytes. __stdcall (RET 8), two pointer
// arguments, return value in EAX.
//
// 16-byte record copy of the 0x004c1xxx container helper family. When the
// destination pointer is non-null, the four dwords of the source record are
// copied field by field (MSVC shuttles the first three through EDX and
// reloads the last through the dead source register ECX). The destination
// stays homed in EAX, matching a destination-returning helper whose result
// the caller may ignore.
struct Record004c1de0 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
};

extern "C" Record004c1de0 * __stdcall sub_004c1de0(Record004c1de0 *destination,
                                                   Record004c1de0 *source) {
    if (destination != 0) {
        destination->field_00 = source->field_00;
        destination->field_04 = source->field_04;
        destination->field_08 = source->field_08;
        destination->field_0c = source->field_0c;
    }
    return destination;
}
