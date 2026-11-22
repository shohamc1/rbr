// sub_004ce3f0: EXE 0x004ce3f0, 43 bytes. __stdcall with one dword argument.
//
// Membership test against the 80-entry dword table at 0x007a5820-0x007a5960
// (just above the 0x18-stride record array served by sub_004ce3d0): scans
// the table with EAX as the cursor, returns 0 on the first equal entry, and
// 1 when the cursor passes the end sentinel 0x007a5960. The compiler pads
// to the 16-byte loop head at 0x004ce400 with a 7-byte LEA ESP,[ESP+0].
// Called from the 0x004ce4xx container code with the dword from +0x50.
extern "C" unsigned int gTable007a5820[0x50]; // EXE .data 0x007a5820

extern "C" int __stdcall sub_004ce3f0(unsigned int value) {
    for (int i = 0; i < 0x50; ++i) {
        if (value == gTable007a5820[i]) {
            return 0;
        }
    }
    return 1;
}
