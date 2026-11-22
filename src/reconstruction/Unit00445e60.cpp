// sub_00445e60: __thiscall member with one stack
// argument (RET 4), stored by sub_0044af60 into five consecutive dispatch
// tables at 0x0044c3ba/0x0044c3f5/0x0044c42f/0x0044c46a/0x0044c4a5, so it
// is reached virtually. Copies a 9-dword (0x24-byte) record from the
// caller's record into the embedded record at +0xd38: the struct
// assignment lowers to ESI/EDI/ECX=9 REP MOVSD. MSVC 7.1 (/O2 /Zl).

struct Record00445e60 {
    unsigned int words[9];
};

class Object00445e60 {
public:
    unsigned char unknown_00[0xd38];
    Record00445e60 field_d38;

    void sub_00445e60(const Record00445e60& record);
};

#include "GamePlatform0044a3c0.h"

// GamePlatform0044a3c0 command-line setter, called by game main (sub_00471110)
// with the process command line. Assigns the argument to the member string at
// offset 0xd64. The reference tail-calls the MSVCP71 import slot 0x0070c2f8,
// std::basic_string<char>::operator=(const basic_string&), with ECX advanced
// to the member and the caller's argument left on the stack.

// sub_00447710: MSVC 7.1 (/O2 /Zl).
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
// (shared class shape with Unit004491e0.cpp); callers pass wrapper+4.

class Node00449270 {
public:
    void* field_00;   // node link, +4 in the wrapper
    void* field_04;   // node link, +8 in the wrapper
    void* field_08;   // owning list pointer, +0xc in the wrapper

    void sub_00447710();   // unlink, EXE 0x447710
};

// sub_004477b0: MSVC 7.1 (/O2 /Zl).
//
// Intrusive-list unlink method for the wrapper class whose finalizer body is
// sub_004491e0 (vtable 0x0070e964) and whose deleting finalizer sub_00449290
// sits in vtable slot 0. The wrapper is a polymorphic head at +0 followed by
// this node base at +4: links at +4/+8 in the wrapper (+0/+4 here) and the
// owning list pointer at +0xc in the wrapper (+8 here). Sibling records keep
// their link pair at +4/+8; the owner record keeps its tail pointer at +0,
// head pointer at +4, and use counter at +8 (decremented by the finalizer
// sub_004491e0 before tail-jumping here). Unlinks this node, repairs the
// neighbor and owner links, then clears the three fields; the single
// trailing clear is duplicated by the compiler into each exit path.
// Byte-identical body to sub_0043d920 and the template siblings
// sub_00447710/sub_00447850/sub_004478f0. __thiscall member of Node004491e0
// (shared class shape with Unit004491e0.cpp); callers pass wrapper+4.

class Node004491e0 {
public:
    void* field_00;   // node link, +4 in the wrapper
    void* field_04;   // node link, +8 in the wrapper
    void* field_08;   // owning list pointer, +0xc in the wrapper

    void sub_004477b0();   // unlink, EXE 0x4477b0
};

// sub_00447850: MSVC 7.1 (/O2 /Zl).
//
// Intrusive-list unlink method for the wrapper class whose finalizer body is
// sub_00449200 (vtable 0x0070e970). The wrapper is a polymorphic head at +0
// followed by this node base at +4: links at +4/+8 in the wrapper (+0/+4
// here) and the owning list pointer at +0xc in the wrapper (+8 here).
// Sibling records keep their link pair at +4/+8; the owner record keeps its
// tail pointer at +0, head pointer at +4, and use counter at +8 (decremented
// by the finalizer sub_00449200 before tail-jumping here). Unlinks this
// node, repairs the neighbor and owner links, then clears the three fields;
// the single trailing clear is duplicated by the compiler into each exit
// path. Byte-identical body to sub_0043d920 and the template siblings
// sub_004477b0/sub_00447710/sub_004478f0. __thiscall member of Node00449200
// (shared class shape with Unit004491e0.cpp); callers pass wrapper+4.

class Node00449200 {
public:
    void* field_00;   // node link, +4 in the wrapper
    void* field_04;   // node link, +8 in the wrapper
    void* field_08;   // owning list pointer, +0xc in the wrapper

    void sub_00447850();   // unlink, EXE 0x447850
};

// sub_004478f0: MSVC 7.1 (/O2 /Zl).
//
// Intrusive-list unlink method for the wrapper class whose finalizer body is
// sub_00449250 (vtable 0x0070e97c). The wrapper is a polymorphic head at +0
// followed by this node base at +4: links at +4/+8 in the wrapper (+0/+4
// here) and the owning list pointer at +0xc in the wrapper (+8 here).
// Sibling records keep their link pair at +4/+8; the owner record keeps its
// tail pointer at +0, head pointer at +4, and use counter at +8 (decremented
// by the finalizer sub_00449250 before tail-jumping here). Unlinks this
// node, repairs the neighbor and owner links, then clears the three fields;
// the single trailing clear is duplicated by the compiler into each exit
// path. Byte-identical body to sub_0043d920 and the template siblings
// sub_004477b0/sub_00447710/sub_00447850. __thiscall member of Node00449250
// (shared class shape with Unit004491e0.cpp); callers pass wrapper+4.

class Node00449250 {
public:
    void* field_00;   // node link, +4 in the wrapper
    void* field_04;   // node link, +8 in the wrapper
    void* field_08;   // owning list pointer, +0xc in the wrapper

    void sub_004478f0();   // unlink, EXE 0x4478f0
};

// FUNCTION: RBR 0x00445e60
void Object00445e60::sub_00445e60(const Record00445e60& record) {
    field_d38 = record;
}

// FUNCTION: RBR 0x00446bd0
void GamePlatform0044a3c0::sub_00446bd0(const std::string& arguments) {
    commandLine = arguments;
}

// FUNCTION: RBR 0x00447710
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

// FUNCTION: RBR 0x004477b0
void Node004491e0::sub_004477b0() {
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

// FUNCTION: RBR 0x00447850
void Node00449200::sub_00447850() {
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

// FUNCTION: RBR 0x004478f0
void Node00449250::sub_004478f0() {
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
