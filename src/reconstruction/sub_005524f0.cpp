// sub_005524f0: EXE 0x005524f0, 11 bytes. __cdecl thunk, no arguments,
// registered as an event callback: EXE 0x00552642 pushes this address into
// virtual slot 0x5c of an object with (0xb, esi, 1, callback). Loads the
// object pointer stored at EXE 0x007b17cc into ECX and tail-jumps to the
// __thiscall member sub_0056e4a0 (0x0056e4a0, 72 bytes, no stack arguments,
// bare RET), so MSVC 7.1 /O2 emits the two-instruction thunk below. Compiled
// with /O2 /Zl.

// Callee class declaration for the unresolved sub_0056e4a0; manifest entry
// pending (see build/harness/reports/cluster7.jsonl).
class Object0056e4a0 {
public:
    void sub_0056e4a0();
};

extern "C" void __cdecl sub_005524f0() {
    ((Object0056e4a0 *)*(unsigned int *)0x007b17cc)->sub_0056e4a0();
}
