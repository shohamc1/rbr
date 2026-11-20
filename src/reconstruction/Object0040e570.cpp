// Object0040e570 members compiled with MSVC 7.1 (/O2 /Zl). The class and its
// merge evidence are in Object0040e570.h; the members that Clang compiles are
// in Object0040e570Clang.cpp.
#include "Object0040e570.h"

extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)

void Object0040e570::sub_0040e740(unsigned char value) { field_6e0 = value; }
void Object0040e570::sub_0040e750(unsigned char value) { field_6e1 = value; }

// Copies eight dwords with forward REP MOVSD; see analysis/notes.md.
void Object0040e570::sub_0040e900(const unsigned int* values) { memcpy(field_6c0, values, 32); }

void Object0040e570::sub_0040ea00() {
    Inner0040ea00* value = inner;
    value->field_38 = 0;
    value->field_34 = 0;
    value->field_30 = 0;
    value->field_3c = 0;
    value->field_40 = 0;
    value->field_44 = 0;
    value->field_48 = 0;
    value->field_4c = 0;
    value->field_64 = 0;
    value->field_68 = 0;
    value->field_6c = 0;
    value->field_70 = 0;
    value->field_74 = 0;
}

unsigned char Object0040e570::sub_0040eb90() { return inner->field_10; }
