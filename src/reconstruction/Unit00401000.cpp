#include "layout.h"
struct Object00401000 {
    unsigned char unknown_00[12];
    unsigned int field_0c;
    unsigned int sub_00401000();
};
LAYOUT_OFFSET(Object00401000, field_0c, 12);

struct Object00401010 { unsigned char unknown_00[4]; unsigned char field_04; unsigned char sub_00401010(); };
#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
typedef char field_offset_check[offsetof(Object00401010, field_04) == 4 ? 1 : -1];

// sub_00401020: MSVC 7.1 (/O2 /Zl).
//
// thiscall predicate on the receiver: loads the dword at +0x4, zero-extends a
// != 0 test into EAX (XOR EAX,EAX; TEST EDX,EDX; SETNZ AL), bare RET. The
// class is the header whose +0x4 slot holds a pointer-or-count tested by
// callers; see sub_004021e0/sub_004022f0 which free the same slot in their
// deleting destructors (vtables 0x0076f3f0/0x0076f3e8).

struct Object00401020 {
    unsigned char field_00[0x4];
    void* field_04;
    bool sub_00401020() const;
};

// FUNCTION: RBR 0x00401000
unsigned int Object00401000::sub_00401000() { return field_0c; }

// FUNCTION: RBR 0x00401010
unsigned char Object00401010::sub_00401010() { return field_04; }

// FUNCTION: RBR 0x00401020
bool Object00401020::sub_00401020() const {
    return field_04 != 0;
}
