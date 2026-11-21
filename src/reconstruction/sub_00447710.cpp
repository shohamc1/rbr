// sub_00447710: EXE 0x00447710, 93 bytes. MSVC 7.1 (/O2 /Zl).
//
// Intrusive-list unlink method for the wrapper class whose finalizer body is
// sub_00449270 (vtable 0x0070e988). The wrapper is a polymorphic head at +0
// followed by this node base at +4: links at +4/+8 in the wrapper (+0/+4
// here) and the owning list pointer at +0xc in the wrapper (+8 here).
// Sibling records keep their link pair at +4/+8; the owner record keeps its
// tail pointer at +0, head pointer at +4, and use counter at +8 (decremented
// by the finalizer sub_00449270 before tail-jumping here). Unlinks this
// node, repairs the neighbor and owner links, then clears the three fields;
// the single trailing clear is duplicated by the compiler into each exit
// path. Byte-identical body to sub_0043d920 and the template siblings
// sub_004477b0/sub_00447850/sub_004478f0. __thiscall member of Node00449270
// (shared class shape with sub_00449270.cpp); callers pass wrapper+4.

class Node00449270 {
public:
    void* field_00;   // node link, +4 in the wrapper
    void* field_04;   // node link, +8 in the wrapper
    void* field_08;   // owning list pointer, +0xc in the wrapper

    void sub_00447710();   // unlink, EXE 0x447710
};

void Node00449270::sub_00447710() {
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
