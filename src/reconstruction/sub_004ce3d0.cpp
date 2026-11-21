// sub_004ce3d0: EXE 0x004ce3d0, 17 bytes. __stdcall with one dword argument.
//
// Returns the address of index-th element of the 0x18-stride record array
// based at 0x007186c0: the two-LEA chain scales the index by 24
// (LEA EAX,[EAX+EAX*2]; LEA EAX,[EAX*8+0x007186c0]). Callers in the
// 0x004ce4xx-0x004ce5xx container code index the same array; the value
// table scanned by sub_004ce3f0 sits just above it at 0x007a5820.
struct Record004ce3d0 {
    unsigned char bytes[0x18];
};

extern "C" Record004ce3d0 * __stdcall sub_004ce3d0(unsigned int index) {
    return (Record004ce3d0 *)0x007186c0 + index;
}
