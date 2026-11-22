// sub_0061e6bf: EXE 0x0061e6bf, 122 bytes. MSVC 7.1 (/O2 /Zl) with the
// file-local size pragma (POP ECX cleanups, per-iteration state reloads).
//
// Derived teardown member of the Object0061d989 hierarchy: stores the
// derived vtable word 0x007340d4 at +0; when the count at +0xf4 and the
// record base at +0xf8 are both nonzero, walks the outer cursor range
// +0xd4..+0xd8 and the inner cursor range +0xc8..+0xd0, freeing the +4
// word of each successive 8-byte record and advancing the record walker by
// 8 (the walker is not reset between outer iterations). Then frees the
// pointers at +0xf0 and +0xf8 and tail-jumps to the base teardown member
// sub_0061d989 with this in ECX. __thiscall, no stack arguments. Callers:
// 0x00627fec, 0x00628754, 0x00628788, 0x006287bc, 0x006287f0, 0x00628824.

#pragma optimize("s", on)

extern "C" void __cdecl sub_006efdd0(void* memory);

class Object0061d989 {
public:
    void sub_0061d989();
};

class Object0061e6bf {
public:
    void* vtable_00;
    unsigned char unknown_04[0xc4];
    unsigned int field_c8;
    unsigned int field_cc;
    unsigned int field_d0;
    unsigned int field_d4;
    unsigned int field_d8;
    unsigned char unknown_dc[0x14];
    void* field_f0;
    unsigned int field_f4;
    void* field_f8;

    void sub_0061e6bf();
};

void Object0061e6bf::sub_0061e6bf() {
    vtable_00 = (void*)0x007340d4;
    if (field_f4 != 0) {
        void* base = field_f8;
        if (base != 0) {
            char* record = (char*)base;
            for (unsigned int i = field_d4; i < field_d8; i++) {
                for (unsigned int j = field_c8; j < field_d0; j += 4) {
                    sub_006efdd0(*(void**)(record + 4));
                    record += 8;
                }
            }
        }
    }
    sub_006efdd0(field_f0);
    sub_006efdd0(field_f8);
    ((Object0061d989*)this)->sub_0061d989();
}
