// sub_0067f514: EXE 0x0067f514, 30 bytes. __stdcall with two pointer
// arguments (RET 8, C symbol _sub_0067f514@8).
//
// Forwards to sub_00679212: (a + 0x14, a + 0x18, *(b + 0x14), *(b + 0x18)).
// sub_00679212 (0x00679212, 256 bytes) ends in RET 0x10: it pops its four
// stack words itself, so this caller is a plain pass-through of its return
// value with no stack cleanup. The reference never loads ECX either:
// sub_00679212 is thiscall-shaped (it reads ECX as its node-table owner),
// and this function relies on its caller leaving the owner in ECX — a
// register input like sub_00411bc0's, so the C-level call leaves ECX alone.
// Callers: 0x0067f54a (sub_0067f532), 0x0068714b, 0x00687231, 0x006872d2,
// 0x00687336.
//
// The two value fields are pushed straight from memory rather than buffered
// in registers: the file-local size pragma selects the memory-push form.
//
// NONMATCHING (best draft, score 2): everything matches except the lea
// scratch for the second argument — this draft emits lea ecx,[eax+0x18];
// push ecx where the reference has lea edx,[eax+0x18]; push edx. MSVC
// 13.10.3077 picks ECX for the single call temp in every shape tried:
// pointer/unsigned argument expressions, locals in both orders, plain /O2,
// "s" and "gs" pragmas, and a thiscall-member definition (which still
// reuses ECX after this dies). Reproducing EDX likely needs a second
// surviving temp for the first argument that folds into ADD EAX,0x14.

#pragma optimize("s", on)

extern "C" int __stdcall sub_00679212(unsigned int out1, unsigned int out2,
                                      unsigned int arg2, unsigned int arg3);

extern "C" int __stdcall sub_0067f514(void *arg0, void *arg1) {
    unsigned int out2 = (unsigned int)arg0 + 0x18;
    unsigned int out1 = (unsigned int)arg0 + 0x14;
    return sub_00679212(out1, out2,
                        *(unsigned int *)((char *)arg1 + 0x14),
                        *(unsigned int *)((char *)arg1 + 0x18));
}
