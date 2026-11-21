// sub_005475f0: EXE 0x005475f0, 34 bytes. __cdecl with one dword argument
// (registered as a callback from sub_00547d60 at 0x005478b4). When the
// argument is zero: sets the dword flag at EXE 0x0163edbc to 1 (sub_00545b50
// clears it) and calls virtual slot 1 of the object at EXE 0x0163f0a8 with
// (0x5b, 0) through CALL [EAX+4] — same receiver class as sub_00545c50.
// The flag store rides in the first argument's value computation: that is
// the one source position that lands it inside the push run (PUSH 0,
// store, MOV EAX,[ECX], PUSH 0x5b) as the reference schedules it.
// Compiled with MSVC 7.1 (/O2 /Zl).

class Object0163f0a8 {
public:
    virtual void sub_007264e4_slot0();
    virtual void slot_04(unsigned int a, unsigned int b);
};

extern "C" void __cdecl sub_005475f0(unsigned int argument) {
    if (argument == 0) {
        Object0163f0a8 *receiver = *(Object0163f0a8 **)0x0163f0a8;
        receiver->slot_04(0x5b, (*(unsigned int *)0x0163edbc = 1, 0));
    }
}
