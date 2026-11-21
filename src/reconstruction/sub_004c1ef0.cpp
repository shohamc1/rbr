// sub_004c1ef0: EXE 0x004c1ef0, 9 bytes. __cdecl, one pointer argument.
//
// Scalar destroy helper for the element class whose destructor lives at EXE
// 0x004c4170 ("add ecx,8; jmp [__imp_??1?$basic_string@DU?$char_traits@D@std@@
// V?$allocator@D@2@@std@@QAE@XZ]"): that class embeds the 0x1c-byte MSVCP71
// basic_string at offset +8 and its destructor forwards to the imported
// string destructor. Loading the element pointer into ECX and tail-jumping
// to the thiscall destructor is MSVC's codegen for a destroy(p) { p->~T(); }
// wrapper; no callee-side manifest entry exists yet (9-byte body).
class Object004c4170 {
public:
    ~Object004c4170();
};

extern "C" void __cdecl sub_004c1ef0(Object004c4170 *object) {
    object->~Object004c4170();
}
