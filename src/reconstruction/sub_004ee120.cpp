// sub_004ee120: EXE 0x004ee120, 8 bytes. __stdcall with three pointer-sized
// stack arguments (RET 0xc): returns 1. No direct callers are known; the
// shape fits a boolean dispatch-table slot that ignores its inputs.
extern "C" int __stdcall sub_004ee120(void *receiver, void *argument0, void *argument1)
{
    (void)receiver;
    (void)argument0;
    (void)argument1;
    return 1;
}
