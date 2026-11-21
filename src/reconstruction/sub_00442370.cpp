// sub_00442370: EXE 0x00442370, 32 bytes. __thiscall member (RET 0xc, three
// stack arguments) of the intrusive list/node subsystem, forwarding to the
// pair-copy helper sub_0043f350 (EXE 0x0043f350, cdecl, reads
// (begin, end, out) over 8-byte records; parameters beyond the third are
// declared but unused at every call site). The member passes
// (begin, end, out, this, out): ECX (this) and the third argument are pushed
// as the helper's trailing arguments. The third parameter is
// volatile-qualified: the two argument reads stay separate loads (no CSE),
// which also pins the whole push-run schedule — EAX/EDX carry out/end above
// the run and ECX reloads out for the third argument. Sibling shapes:
// sub_00443510 and sub_00443360 forward ECX and a repeated argument the
// same way. MSVC 7.1 (/O2 /Zl).

extern "C" void __cdecl sub_0043f350(void* begin, void* end, void* out,
                                     void* fourth, void* fifth);

class Object00442370 {
public:
    void sub_00442370(void* begin, void* end, void* volatile out);
};

void Object00442370::sub_00442370(void* begin, void* end, void* volatile out) {
    sub_0043f350(begin, end, out, this, out);
}
