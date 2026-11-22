// sub_004213b0: EXE 0x004213b0, 62 bytes. MSVC 7.1 (/O2 /Zl).
//
// Recursive release of the hierarchical intrusive list built from the node
// class whose string destructor is sub_00421ff0. The node keeps its sibling
// chain at +0, a backlink at +4 and its child chain at +8, the 0x1c-byte
// MSVCP71 basic_string at +0x10, a byte flag at +0x2c and the sentinel flag
// byte at +0x2d (same shape as the sub_00499270 family, whose sentinels sit
// at +0x1d). __thiscall on the container in ECX with the head node as the
// one stack argument: while the walker is not the sentinel, release its
// child chain recursively, advance the walker to the sibling at +0, destroy
// the lagging node's embedded string through sub_00421ff0, free it through
// the cdecl operator-delete thunk sub_006efdd0, then adopt the walker as
// the next lagging node. Called from sub_00421720 at 0x00421742.
//
// The class below reuses the Object00421ff0 name so the destructor call
// mangles to the same ??1 symbol that sub_00421ff0.cpp defines; the field
// view here is the richer one shared by this family.
//
// Register encoding: the parameter itself must be the node that lags one
// step behind the walker (destroyed and freed each iteration, refreshed at
// the body end), while a separate `current` walker is initialized from it;
// reusing the parameter for the lagging node pins the entry to
// `mov edi,[esp+0x10]; mov esi,edi` with the upfront PUSH EDI, where a
// second local swaps the ESI/EDI roles. An explicit destructor call on a
// class-typed parameter keeps the deleting-dtor COMDAT unemitted.

#include "msvcp71.h"

class Object00421ff0 {
public:
    Object00421ff0* field_00;  // sibling chain, +0
    void* field_04;            // backlink, +4
    Object00421ff0* field_08;  // child chain, +8
    unsigned char field_0c[4];
    std::string field_10;      // destroyed by sub_00421ff0
    unsigned char field_2c;
    unsigned char flag_2d;     // sentinel flag
    ~Object00421ff0();
};

extern "C" void __cdecl sub_006efdd0(void* memory);  // 0x006efdd0, operator delete thunk

typedef char node004213b0_size_assertion[(sizeof(Object00421ff0) == 0x30) ? 1 : -1];
typedef char node004213b0_flag_assertion[
    (((unsigned long)&((Object00421ff0*)0)->flag_2d) == 0x2d) ? 1 : -1];

class Object004213b0 {
public:
    void sub_004213b0(Object00421ff0* node);
};

void Object004213b0::sub_004213b0(Object00421ff0* node) {
    Object00421ff0* current = node;
    while (current->flag_2d == 0) {
        sub_004213b0(current->field_08);
        current = current->field_00;
        node->~Object00421ff0();
        sub_006efdd0(node);
        node = current;
    }
}
