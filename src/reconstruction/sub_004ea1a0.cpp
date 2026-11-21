// sub_004ea1a0: EXE 0x004ea1a0, 8 bytes. __stdcall with one pointer-sized
// stack argument (RET 4): returns 1. No direct callers are known; the shape
// fits a boolean dispatch-table slot that ignores its receiver.
extern "C" int __stdcall sub_004ea1a0(void *receiver)
{
    (void)receiver;
    return 1;
}
