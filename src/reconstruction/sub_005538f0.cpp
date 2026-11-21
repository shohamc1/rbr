// sub_005538f0: EXE 0x005538f0, 23 bytes. __cdecl, one dword argument, plain
// RET. Predicate on a state code: true when the argument is 0 or 1 (unsigned
// compare, JBE covers both contiguous cases) or exactly 10 (0xa). No direct
// references found in the EXE; dispatch is unexplained (registration or table
// evidence pending). Compiled with MSVC 7.1 (/O2 /Zl); the sparse switch keeps
// the JBE/JNE chain with the case block inline and the default out-of-line.
extern "C" int __cdecl sub_005538f0(unsigned int value) {
    switch (value) {
    case 0:
    case 1:
    case 10:
        return 1;
    default:
        return 0;
    }
}
