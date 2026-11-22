// sub_0043a270: EXE 0x0043a270, 30 bytes. Compiled with MSVC 7.1 (/O2 /Zl).
//
// Fill loop for 4-byte elements: copies *value into every slot of
// [first, last). The value is reloaded from *value on every iteration
// (MOV ESI,[EDX] inside the loop), so value may alias the filled range.
// Callers at 0x0043a74e and 0x0043a782 push three pointers and clean up
// with ADD ESP,0xc: cdecl. Part of the allocator/vector helper family
// around sub_0043a290/sub_0043a2c0.
extern "C" void __cdecl sub_0043a270(unsigned int* first, unsigned int* last,
                                      const unsigned int* value) {
    while (first != last) {
        *first = *value;
        ++first;
    }
}
