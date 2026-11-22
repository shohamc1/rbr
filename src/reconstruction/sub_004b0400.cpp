// Object004b0400 constructor: EXE 0x004b0400, 13 bytes. __thiscall with
// ECX-in/EAX-out (the MOV EAX,ECX prologue identifies a constructor).
// Zeroes three dword fields at +0, +4, +8; the +4 store is emitted first.
// No direct callers in Ghidra; consistent with vtable or data dispatch.
// Hypothesis for the 0x004b04xx family: an intrusive list header with
// head +0, tail +4, count +8 (tail-append sub_004b04b0 links those fields
// on nodes with next +0, prev +4, owner +8).

struct Object004b0400 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    Object004b0400();
};

Object004b0400::Object004b0400() {
    field_04 = 0;
    field_00 = 0;
    field_08 = 0;
}
