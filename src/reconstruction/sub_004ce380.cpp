// sub_004ce380: EXE 0x004ce380, 19 bytes. thiscall member returning float.
//
// Unsigned-to-float view of the dword field at +0x50 (the same field the
// dword getters at 0x004ce368/0x004ce378 read). MSVC 7.1 expands the
// conversion into FILD plus a 2^32 correction when the high bit is set; the
// __real@4f800000 bias resolves to the shared pool float at 0x00776a38.
#include "layout.h"

class Object004ce380 {
public:
    float sub_004ce380();

    char pad_00[0x50];
    unsigned int field_50;
};

LAYOUT_OFFSET(Object004ce380, field_50, 0x50);

float Object004ce380::sub_004ce380() {
    return (float)field_50;
}
