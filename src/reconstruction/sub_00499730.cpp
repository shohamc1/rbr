// sub_00499730: EXE 0x00499730, 15 bytes. __stdcall leaf.
// Bytes-to-KiB ceiling divide: (n + 0x3ff) >> 10. Referenced from the data
// tables at EXE 0x0070f3b4 and 0x0071385c (vtable slots), so calls are
// indirect and Ghidra lists no direct callers.

extern "C" unsigned int __stdcall sub_00499730(unsigned int bytes)
{
    return (bytes + 0x3ff) >> 10;
}
