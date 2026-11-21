// Three-way compare of two packed 64-bit values passed as four dwords on the
// stack: value A = (a_hi:a_lo) at [esp+8]/[esp+4], value B = (b_hi:b_lo) at
// [esp+0x10]/[esp+0xc]. Returns 1, -1, or 0 for A>B, A<B, A==B. Callers in
// sub_00432c40 (0x0043323d, 0x00433308, 0x00433380, 0x004334fd) push four
// dwords and test the sign of the result (`test eax,eax; jl`). The compared
// values look like four-u16 version numbers; see the packer sub_004336b0.
// Compiled with MSVC 7.1 (/O2 /Zl): the high dwords are compared once and
// both flag senses reused; the low compare ends in SBB.
extern "C" int __cdecl sub_004336d0(unsigned int a_lo, unsigned int a_hi,
                                    unsigned int b_lo, unsigned int b_hi) {
    if (a_hi > b_hi) return 1;
    else if (a_hi < b_hi) return -1;
    else if (a_lo > b_lo) return 1;
    else return a_lo < b_lo ? -1 : 0;
}
