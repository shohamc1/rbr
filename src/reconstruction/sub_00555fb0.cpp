// sub_00555fb0: EXE 0x00555fb0, 55 bytes. __thiscall member returning 1, no
// stack arguments. When the dword at +0x00 is 3 or 5 (SUB 3 / SUB 2 chain),
// stores the 16-bit value (0xfffe - word@+0x794) through the pointer at
// +0xc98; otherwise stores a zero word through it. The 16-bit arithmetic
// (MOV DX,0xfffe / SUB DX,mem16) comes from unsigned-short operands. Direct
// __thiscall callers at EXE 0x00556d97, 0x0055788e, and 0x005b3803. The
// pointer at +0xc98 names a record whose word 0 is an index: EXE 0x00553972
// scales it by 0x3eb0, with 0x7b as a special case. Compiled with MSVC 7.1
// (/O2 /Zl).

class Object00555fb0 {
public:
    unsigned int field_00;
    unsigned char unknown_04[0x790];
    unsigned short field_794;
    unsigned char unknown_796[0x502];
    unsigned short *field_c98;
    int sub_00555fb0();
};

typedef char Object00555fb0_offset_794[((unsigned int)&(((Object00555fb0*)0)->field_794) == 0x794) ? 1 : -1];
typedef char Object00555fb0_offset_c98[((unsigned int)&(((Object00555fb0*)0)->field_c98) == 0xc98) ? 1 : -1];

int Object00555fb0::sub_00555fb0() {
    switch (field_00) {
    case 3:
    case 5:
        *field_c98 = (unsigned short)(0xfffe - field_794);
        break;
    default:
        *field_c98 = 0;
        break;
    }
    return 1;
}
