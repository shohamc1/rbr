// Object005c2080: EXE 0x005c2080, 32 bytes. __thiscall, ECX in and EAX = this
// out: a constructor (or an equivalent reset member). Six dword stores:
// +0x0, +0x4, +0x8 = -1 (single OR ECX,-1 value reused three times),
// +0xc, +0x14 = 1 (MOV ECX,1 reused), +0x10 = 0 immediate last.
// Called from sub_005c1840 at 0x005c1bc4 and 0x005c1c94.
//
// Encoding lever: MSVC 7.1 /O2 keeps the stores in statement order here, so
// the +0x14 assignment must precede the +0x10 assignment in the body; the
// declaration-order body interleaves the 0 immediate between the two
// ECX=1 stores ([eax+0xc], [eax+0x10]=0, [eax+0x14]) and misses by one
// instruction swap.

struct Object005c2080 {
    unsigned int field_00;   // -1
    unsigned int field_04;   // -1
    unsigned int field_08;   // -1
    unsigned int field_0c;   // 1
    unsigned int field_10;   // 0
    unsigned int field_14;   // 1

    Object005c2080();
};

Object005c2080::Object005c2080() {
    field_00 = -1;
    field_04 = -1;
    field_08 = -1;
    field_0c = 1;
    field_14 = 1;
    field_10 = 0;
}
