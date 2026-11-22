// sub_004c1da0: __cdecl, three pointer arguments,
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

// sub_004c1de0: __stdcall (RET 8), two pointer
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

// sub_004c1e40: __stdcall (RET 8), two pointer
// arguments.
//
// 0x124-byte record copy of the 0x004c1xxx container helper family. When the
// destination pointer is non-null, the record is copied with the MSVC memcpy
// intrinsic: EDI receives the destination (tested for null), the source is
// loaded into ESI after the branch, ECX = 0x49 dwords, REP MOVSD.
extern "C" void * __cdecl memcpy(void *, const void *, unsigned int);
#pragma intrinsic(memcpy)

// sub_004c1e70: __cdecl, three pointer arguments,
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

// sub_004c1eb0: __cdecl, three pointer arguments,
// return value in EAX (the advanced destination cursor).
//
// Forward 0x124-byte record copy of the 0x004c1xxx container helper family
// (caller sub_004c2bc0, four call sites 0x004c2d06-0x004c2e67). While the
// source cursor has not reached the end cursor, a non-null destination
// receives the whole 0x124-byte record (MSVC memcpy intrinsic: ECX = 0x49
// dwords, ESI = source, EDI = destination, REP MOVSD). Both cursors then
// advance one record.
struct Record004c1eb0 { unsigned char bytes[0x124]; };

// sub_004c1ef0: __cdecl, one pointer argument.
//
// Scalar destroy helper for the element class whose destructor lives at EXE
// 0x004c4170 ("add ecx,8; jmp [__imp_??1?$basic_string@DU?$char_traits@D@std@@
// V?$allocator@D@2@@std@@QAE@XZ]"): that class embeds the 0x1c-byte MSVCP71
// basic_string at offset +8 and its destructor forwards to the imported
// string destructor. Loading the element pointer into ECX and tail-jumping
// to the thiscall destructor is MSVC's codegen for a destroy(p) { p->~T(); }
// wrapper; no callee-side source exists yet (9-byte body).
class Object004c4170 {
public:
    ~Object004c4170();
};

// sub_004c1f00: __thiscall member (ECX = this)
// with one dword stack argument (RET 4), pointer result in EAX.
//
// Array accessor of the 0x004c1xxx container family: the record array base
// sits at +4 and each record is 0x50 bytes (MSVC strength-reduces
// index * 0x50 to LEA x5 plus SHL 4). No direct callers in Ghidra;
// consistent with dispatch through a table such as 0x007137b4.
struct Record004c1f00 { unsigned char bytes[0x50]; };

class Object004c1f00 {
public:
    unsigned char unknown_00[4];
    Record004c1f00 *field_04;

    Record004c1f00 *sub_004c1f00(unsigned int index);
};

// sub_004c1f20: __thiscall member (ECX = this)
// with one dword stack argument (RET 4), pointer result in EAX.
//
// Byte-identical twin of sub_004c1f00 kept as a separate object per the
// one-function rule: same array accessor shape for the 0x004c1xxx container
// family, record array base at +4, 0x50-byte records (LEA x5 plus SHL 4).
// No direct callers in Ghidra; the original was a second instantiation or
// table slot of the same accessor.
struct Record004c1f20 { unsigned char bytes[0x50]; };

class Object004c1f20 {
public:
    unsigned char unknown_00[4];
    Record004c1f20 *field_04;

    Record004c1f20 *sub_004c1f20(unsigned int index);
};

// sub_004c1f40: __thiscall member (ECX = this)
// with one pointer stack argument (RET 4).
//
// Out-parameter getter of the 0x004c1xxx container family: stores the
// record-array base at +4 through the caller-supplied pointer. No direct
// callers in Ghidra; consistent with dispatch through a table such as
// 0x007137b4.
class Object004c1f40 {
public:
    unsigned char unknown_00[4];
    void *field_04;

    void **sub_004c1f40(void **slot);
};

// FUNCTION: RBR 0x004c1da0
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

// FUNCTION: RBR 0x004c1de0
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

// FUNCTION: RBR 0x004c1e40
extern "C" void __stdcall sub_004c1e40(void *destination, const void *source) {
    if (destination != 0)
        memcpy(destination, source, 0x124);
}

// FUNCTION: RBR 0x004c1e70
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

// FUNCTION: RBR 0x004c1eb0
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

// FUNCTION: RBR 0x004c1ef0
extern "C" void __cdecl sub_004c1ef0(Object004c4170 *object) {
    object->~Object004c4170();
}

// FUNCTION: RBR 0x004c1f00
Record004c1f00 *Object004c1f00::sub_004c1f00(unsigned int index) {
    return field_04 + index;
}

// FUNCTION: RBR 0x004c1f20
Record004c1f20 *Object004c1f20::sub_004c1f20(unsigned int index) {
    return field_04 + index;
}

// FUNCTION: RBR 0x004c1f40
void **Object004c1f40::sub_004c1f40(void **slot) {
    *slot = field_04;
    return slot;
}
