// Object004b04a0 constructor: EXE 0x004b04a0, 16 bytes. __thiscall with
// ECX-in/EAX-out (MOV EAX,ECX prologue identifies a constructor).
// Zeroes four dword fields at +0, +4, +8, +0xc; the +4 store is emitted
// first. Hypothesis for the 0x004b04xx family: this is the 16-byte list
// header of the prefixed-node family whose tail-append sub_004b0520
// links nodes with next +4, prev +8, owner +0xc (head +0, tail +4,
// count +8, spare +0xc).

struct Object004b04a0 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
    Object004b04a0();
};

Object004b04a0::Object004b04a0() {
    field_04 = 0;
    field_00 = 0;
    field_08 = 0;
    field_0c = 0;
}
