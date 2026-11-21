// Object004b0490 constructor: EXE 0x004b0490, 13 bytes. __thiscall with
// ECX-in/EAX-out (MOV EAX,ECX prologue identifies a constructor).
// Byte-identical to the Object004b0400 constructor at 0x004b0400: zeroes
// the dword fields at +0, +4, +8 with the +4 store emitted first.
// Same 0x004b04xx intrusive-list family hypothesis (head +0, tail +4,
// count +8); a separate emitted body implies a separate class or
// translation unit for the original build.

struct Object004b0490 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    Object004b0490();
};

Object004b0490::Object004b0490() {
    field_04 = 0;
    field_00 = 0;
    field_08 = 0;
}
