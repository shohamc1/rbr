// sub_00480920: Default constructor, __thiscall,
// no parameters, returns this in EAX.
//
// Zeroes four dword members at +0x00/+0x04/+0x08/+0x0c from one zeroed
// register. The single incoming reference is a data word at 0x00480bf7
// (inside 0x00480af0's span), consistent with a dispatch or initializer
// table entry; the record layout is body evidence.

class Object00480920 {
public:
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;

    Object00480920();
};

// sub_00480930: Default constructor, __thiscall,
// no parameters, returns this in EAX.
//
// Stores the -1 handle sentinel at +0x20 first (immediate store), then
// zeroes +0x18 and +0x14 from one zeroed register; +0x1c is untouched
// padding. The same record is released by sub_00480950 (frees +0x14/+0x18,
// closes the +0x20 handle) and reopened by sub_004809d0 (CreateFileA on the
// name at +0x24, handle stored at +0x20), so this is the constructor of
// that file-wrapper record. Callers include 0x00463900, 0x0048ab60,
// 0x004d3350, and 0x005c2770.

class Object00480930 {
public:
    unsigned char unknown_00[0x14];
    void* field_14;
    void* field_18;
    unsigned char unknown_1c[4];
    void* field_20;
    char field_24[260];

    Object00480930();
};

// sub_00480950: __thiscall member, no parameters.
//
// Release path of the file-wrapper record constructed by sub_00480930:
// closes the handle at +0x20 unless it holds the -1 sentinel, then frees the
// blocks at +0x18 and +0x14 through the cdecl operator-delete thunk at EXE
// 0x006efdd0 (jmp [0x0070c504]); the two one-argument cdecl calls share one
// ADD ESP,8 cleanup. Callers include 0x00463970, 0x004d3830, and the
// 0x006f3c10-0x006f3c38 thunks.

#include "win32.h"

extern "C" void __cdecl sub_006efdd0(void* block);

class Object00480950 {
public:
    unsigned char unknown_00[0x14];
    void* field_14;
    void* field_18;
    unsigned char unknown_1c[4];
    void* field_20;

    void sub_00480950();
};

// sub_00480980: __stdcall with two stack
// arguments (RET 8); ECX/EDX are never read.
//
// Case-insensitive name lookup over a singly linked list. Walks the link at
// +0x00, comparing the caller's name against the string at record+0x14 where
// the record pointer sits at node+0x08 (MSVCR71 _stricmp, IAT 0x0070c624,
// loaded once into EBX before the loop). A hit returns the node itself when
// its +0x0c word is nonzero, otherwise the substitute pointer at +0x04; a
// exhausted walk returns null. Callers: 0x00480c80 and 0x00480d20.

extern "C" __declspec(dllimport) int __cdecl _stricmp(const char* string1,
                                                      const char* string2);

struct Node00480980 {
    Node00480980* field_00;
    void* field_04;
    char* field_08;
    unsigned int field_0c;
};

// sub_004809d0: __thiscall member, no stack
// arguments, returns bool in AL (SETNZ after CMP EAX,-1).
//
// Reopen path of the file-wrapper record built by sub_00480930: closes the
// handle at +0x20 unless it holds the -1 sentinel (KERNEL32 CloseHandle,
// IAT 0x0070c18c), then opens the name at +0x24 with CreateFileA (IAT
// 0x0070c1b0) as GENERIC_READ(0x80000000), share 1, OPEN_EXISTING(3),
// flags 0x8000080, no template, stores the new handle at +0x20 and reports
// whether it differs from the -1 sentinel. No incoming references are
// known; the layout follows the constructor and sub_00480950.

#include "win32.h"

extern "C" __declspec(dllimport) void* __stdcall CreateFileA(
    const char* file_name, unsigned long desired_access,
    unsigned long share_mode, void* security_attributes,
    unsigned long creation_disposition, unsigned long flags_and_attributes,
    void* template_file);

class Object004809d0 {
public:
    unsigned char unknown_00[0x14];
    void* field_14;
    void* field_18;
    unsigned char unknown_1c[4];
    void* field_20;
    char field_24[260];

    bool sub_004809d0();
};

// sub_00480e80: __thiscall member with one stack
// argument (RET 4), returns this in EAX.
//
// Copies the 16-byte float record from the argument to *this with one
// MOVUPS load/store pair; `this` is homed in EDX plus a 4-byte stack slot
// (PUSH ECX; MOV EDX,ECX; MOV [ESP],EDX; MOV EAX,[ESP]; MOV EAX,EDX).
// Member of the SIMD math family around 0x00480e80-0x00480f60 together with
// sub_00480ec0 and sub_00480ef0 (see sub_00463ff0's note for the family's
// homing signature); no incoming references are known, the layout is body
// evidence.
//
// MSVC 7.1 intrinsic declarations: __m128 is declared WITHOUT the
// __declspec(align(16)) of the genuine xmmintrin.h typedef. With the
// alignment, MSVC 13.10.3077 wraps any intrinsic body in an EBP frame with
// AND ESP,~0xF realignment; without it the MOVUPS pair lowers inline.
//
// NONMATCHING (4 instructions): the arithmetic is exact, but the reference
// stores `this` to the homing slot BEFORE reading the argument and returns
// from EDX (MOV EDX,ECX; MOV [ESP],EDX; MOV ECX,[ESP+8]; MOVUPS XMM0,[ECX];
// MOV EAX,[ESP]; MOVUPS [EAX],XMM0; MOV EAX,EDX), while this draft loads the
// argument first (MOV EAX,ECX; MOV ECX,[ESP+8]; MOVUPS XMM0,[ECX];
// MOV [ESP],EAX; MOV EDX,[ESP]; MOVUPS [EDX],XMM0) with `this` homed in EAX.
// Pointer locals, reference bindings, __fastcall/cdecl helper inlining,
// optimize("s"/"g"), /O1, and /arch:SSE all leave the load hoisted above the
// volatile store; 12 shapes recorded. Same family residual as sub_00463ff0
// and sub_00464080.

typedef struct __declspec(intrin_type) __m128 {
    float m128_f32[4];
} __m128;

extern "C" {
__m128 _mm_loadu_ps(const float* v);
void _mm_storeu_ps(float* v, __m128 a);
}

class Object00480e80 {
public:
    float field_00[4];

    Object00480e80* sub_00480e80(const Object00480e80* src);
};

// sub_00480ea0: __thiscall member with one stack
// argument (RET 4), x87 result in ST(0).
//
// Three-component dot product: this . v with the argument's components
// loaded first (FLD [EAX+8], FMUL [ECX+8], FLD [EAX+4], FMUL [ECX+4], FADDP,
// FLD [EAX], FMUL [ECX], FADDP), so the z product sits deepest on the x87
// stack. x87 codegen: no /arch:SSE. Member of the vector family around
// 0x00480e80-0x00480f60; no incoming references are known, the layout is
// body evidence.

class Object00480ea0 {
public:
    float field_00;
    float field_04;
    float field_08;

    float sub_00480ea0(const Object00480ea0* v);
};

// FUNCTION: RBR 0x00480920
Object00480920::Object00480920() {
    field_00 = 0;
    field_04 = 0;
    field_08 = 0;
    field_0c = 0;
}

// FUNCTION: RBR 0x00480930
Object00480930::Object00480930() {
    field_20 = (void*)-1;
    field_18 = 0;
    field_14 = 0;
}

// FUNCTION: RBR 0x00480950
void Object00480950::sub_00480950() {
    if (field_20 != (void*)-1) {
        CloseHandle(field_20);
    }
    sub_006efdd0(field_18);
    sub_006efdd0(field_14);
}

// FUNCTION: RBR 0x00480980
extern "C" void* __stdcall sub_00480980(const char* name, Node00480980* node) {
    while (node != 0) {
        if (_stricmp(name, node->field_08 + 0x14) == 0) {
            if (node->field_0c != 0) {
                return node;
            }
            return node->field_04;
        }
        node = node->field_00;
    }
    return 0;
}

#pragma optimize("s", on)
// FUNCTION: RBR 0x004809d0
bool Object004809d0::sub_004809d0() {
    if (field_20 != (void*)-1) {
        CloseHandle(field_20);
    }
    void* handle = CreateFileA(field_24, 0x80000000, 1, 0, 3, 0x8000080, 0);
    field_20 = handle;
    if (handle == (void*)-1) {
        return false;
    }
    return true;
}
#pragma optimize("", on)

// NONMATCHING
// FUNCTION: RBR 0x00480e80
Object00480e80* Object00480e80::sub_00480e80(const Object00480e80* src) {
    float* volatile dst = field_00;
    _mm_storeu_ps(dst, _mm_loadu_ps(src->field_00));
    return this;
}

// FUNCTION: RBR 0x00480ea0
float Object00480ea0::sub_00480ea0(const Object00480ea0* v) {
    return field_00 * v->field_00 + field_04 * v->field_04 +
           field_08 * v->field_08;
}
