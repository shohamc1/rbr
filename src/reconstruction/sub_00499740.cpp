// sub_00499740: EXE 0x00499740, 10 bytes. __stdcall leaf.
// KiB to bytes: n << 10. Referenced from the data tables at EXE 0x0070f3b8
// and 0x00713860 (vtable slots), so calls are indirect.

extern "C" unsigned int __stdcall sub_00499740(unsigned int kbytes)
{
    return kbytes << 10;
}
