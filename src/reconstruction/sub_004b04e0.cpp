// Object004b04e0 constructor: EXE 0x004b04e0, 16 bytes. __thiscall with
// ECX-in/EAX-out (MOV EAX,ECX prologue identifies a constructor).
// Byte-identical to the Object004b04a0 constructor at 0x004b04a0: zeroes
// four dword fields at +0, +4, +8, +0xc with the +4 store emitted first.
// Same 16-byte list-header hypothesis as sub_004b04a0; a separate emitted
// body implies a separate class or translation unit for the original
// build.

struct Object004b04e0 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
    Object004b04e0();
};

Object004b04e0::Object004b04e0() {
    field_04 = 0;
    field_00 = 0;
    field_08 = 0;
    field_0c = 0;
}
