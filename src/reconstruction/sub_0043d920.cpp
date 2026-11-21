// sub_0043d920: EXE 0x0043d920, 93 bytes. MSVC 7.1 (/O2 /Zl).
//
// Intrusive-list unlink on the 3-pointer link node embedded at +4 of the
// delegate record built by sub_005ed0f0 (dispatch table at +0). The link
// fields are prev record at +0, next record at +4, and owner at +8; the
// sibling records keep their link pair at +4/+8, and the owner record keeps
// its tail pointer at +0, head pointer at +4, and use counter at +8
// (decremented by the wrapper sub_005ed040 before tail-jumping here with
// record+4 as this). Unlinks this node, repairs the neighbor and owner
// links, then clears the node's three fields; the single trailing clear is
// duplicated by the compiler into each exit path (three byte-identical
// copies, one shared [reg+4] store cross-jumped at 0x0043d95e). The owner
// value loaded for the guard stays in EDX and is reused by the both-links-
// null path; zero lives in ESI. Byte-identical body to the four template
// instantiations sub_004477b0/sub_00447850/sub_004478f0/sub_00447710.
// __thiscall member of Link005ed040 (shared class shape with
// sub_005ed040.cpp).

class Link005ed040 {
public:
    void* field_00;   // prev record, +0 in the link node
    void* field_04;   // next record, +4 in the link node
    void* field_08;   // owner record (tail +0, head +4, count +8)

    void sub_0043d920();
};

void Link005ed040::sub_0043d920() {
    if (field_08 != 0) {
        if (field_00 != 0) {
            *(void**)((char*)field_00 + 8) = field_04;
            if (field_04 == 0) {
                *(void**)field_08 = field_00;
            } else {
                *(void**)((char*)field_04 + 4) = field_00;
            }
        } else if (field_04 != 0) {
            *(void**)((char*)field_04 + 4) = 0;
            *(void**)((char*)field_08 + 4) = field_04;
        } else {
            *(void**)field_08 = 0;
            *(void**)((char*)field_08 + 4) = 0;
        }
    }
    field_00 = 0;
    field_04 = 0;
    field_08 = 0;
}
