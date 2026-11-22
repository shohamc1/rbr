// WinMain, called by the CRT entry point with four stack arguments and RET 0x10.
// Reads the run modes from RichardBurnsRally.ini, checks the SSE CPU feature with
// CPUID, initializes COM, creates the 0x1bd0-byte platform object, hands it the
// command line, and runs the game loop.
//
// Known byte-level blockers, probed against the pinned MSVC 13.10.3077 compiler:
// - CPUID(1) at 0x00471360 (MOV EAX,1; CPUID; use EDX): MSVC 7.1 has no __cpuid
//   intrinsic (warning C4163 probed), so this uses the one inline-assembly
//   block the original must also have used.
// - The std::basic_string constructor and destructor calls go through the
//   MSVCP71 import table. The explicit specialization below reproduces the
//   exact mangled import names and the CALL [__imp_...] form of the reference.
// - C++ EH data and funclets also need checking.

#include "msvcp71.h"
#include "Object0040f9f0.h"
#include "GamePlatform0044a3c0.h"
#include "Object0040e570.h"

// Startup state: the splash flag and window restored by the macro below, the
// single-run guard, and the platform object this function creates.
extern "C" unsigned char gFlag007ca1fc;          // EXE .data 0x007ca1fc
extern "C" void* gWindow007ca1f8;                // EXE .data 0x007ca1f8
extern "C" unsigned int gGuard007ca1f0;          // EXE .data 0x007ca1f0
extern "C" int gLogToStdout;                     // EXE .data 0x007a18f8
extern "C" GamePlatform0044a3c0* gPlatform;      // EXE .data 0x007ca2a0
extern "C" long __stdcall sub_00470f70(void* exceptionPointers);

// KERNEL32/USER32/OLE32 imports; the reference reaches all of them through the
// import address table (CALL [__imp_...]).
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void* filter);
extern "C" __declspec(dllimport) int __stdcall MessageBoxA(void* window, const char* text,
                                     const char* caption, unsigned int type);
extern "C" __declspec(dllimport) int __stdcall ShowCursor(int show);
extern "C" __declspec(dllimport) int __stdcall DestroyWindow(void* window);
extern "C" __declspec(dllimport) int __stdcall SetForegroundWindow(void* window);
extern "C" __declspec(dllimport) int __stdcall SetFocus(void* window);
extern "C" __declspec(dllimport) long __stdcall CoInitialize(void* reserved);
extern "C" __declspec(dllimport) void __stdcall CoUninitialize();

// EXE helpers. The diagnostics draft still uses a stack argument for its handler.
typedef int __cdecl DiagnosticsHandler();
extern "C" int __cdecl sub_0041c8a0(DiagnosticsHandler* handler);
extern "C" bool __cdecl sub_00470f00();
extern "C" void __cdecl sub_00470ca0(int runIntro, void* instance);
extern "C" void __cdecl sub_00470ea0();
extern "C" void __cdecl sub_00470e60();
extern "C" void __cdecl sub_0043b3c0(int kind, int code, const char* message, ...);
extern "C" void* __cdecl sub_006efa32(unsigned int size); // operator new; linker symbol resolves

// The application global at 0x007c9fe8 holds the window object at +0x28.
// Object0040e570 is declared in Object0040e570.h.
struct AppWindow007c9fe8 {
    unsigned char unknown_00[0x28];
    Object0040e570* window;
};
typedef char app_window_field_assertion[
    ((unsigned int)&((AppWindow007c9fe8*)0)->window) == 0x28 ? 1 : -1];
extern "C" AppWindow007c9fe8* gApplication;      // EXE .data 0x007c9fe8

// The ini reader object lives on this function's stack at [esp+0x4]; its
// constructor (sub_0040f9f0) and destructor (sub_0040fa60) are thiscalls.
// The class is shared with Unit0040df40.cpp through Object0040f9f0.h.

// The 0x1bd0-byte platform object created below is declared in
// GamePlatform0044a3c0.h (delete uses vtable slot 0 with argument 1, the
// checked call uses slot 1).

// The reference duplicates this block before the "Could not initialize COM!"
// message and again after SetFocus. A macro keeps the two copies identical.
#define RESTORE_CURSOR_AND_WINDOW() \
    if (gFlag007ca1fc != 0) { \
        gFlag007ca1fc = 0; \
        if (gWindow007ca1f8 != 0) { \
            ShowCursor(1); \
            DestroyWindow(gWindow007ca1f8); \
            gWindow007ca1f8 = 0; \
        } \
    }

#include "layout.h"
struct Object004716c0 { unsigned char unknown_00[4]; unsigned int field_04; void sub_004716c0(); };
LAYOUT_OFFSET(Object004716c0, field_04, 4);

// sub_00471790: MSVC 7.1 (/O2 /Zl).
//
// SSE length of the receiver's 3D vector: loads four components with
// MOVUPS, squares them packed, folds the first three squares with two
// in-place SHUFPS 0x99 rotations and ADDSS, takes SQRTSS, and returns the
// float through x87 (MOVSS to a stack slot, FLD, ST(0) return). thiscall
// member, bare RET. Callers across the EXE (about 90 sites, e.g. 0x0051c017)
// FCOMP the ST(0) result against a radius. Widely shared math helper; the
// receiver record is a bare three-float vector (12+ bytes) at +0.
//
// MSVC 7.1 intrinsic declarations: __m128 is declared WITHOUT the
// __declspec(align(16)) of the genuine xmmintrin.h typedef; with the
// alignment MSVC 13.10.3077 wraps the body in an EBP frame with
// AND ESP,~0xF realignment.
//
// NONMATCHING (score 19, 81 vs 52 bytes): the load, MULPS, the copy of the
// squares into the accumulator register, the first in-place SHUFPS 0x99 and
// ADDSS, and the SQRTSS/MOVSS/FLD return all reproduce (the copy compiles
// as MOVAPS; the reference uses the MOVUPS encoding). The residual is the
// same build-mode difference as the SIMD math family
// (sub_00463ff0/sub_00480e80/sub_004eb160/sub_00480ec0): the reference
// keeps `this` homed in a 4-byte stack slot (SUB ESP,8; MOV [ESP],ECX;
// MOV EAX,[ESP]) and runs the SECOND rotation in place in XMM0 with the
// accumulator staying in XMM1, while MSVC 7.1 /O2 copies the second
// rotation to XMM2, re-homes the accumulator to XMM0, and spills once
// (MOVAPS to stack, which also forces the EBP+AND ESP,~0xF frame).
// Seven shapes tried (reused vs fresh chain and accumulator names, nested
// vs staged shuffles, an unrolled-candidate loop that stays a loop, and
// #pragma optimize("s")); all produce the same round-2 copies. With or
// without /arch:SSE makes no difference (the flag request was withdrawn).
// The this-homing itself is reproduced with a volatile pointer local, as
// in sub_00480ec0.

typedef struct __declspec(intrin_type) __m128 {
    float m128_f32[4];
} __m128;

extern "C" {
__m128 _mm_loadu_ps(const float *v);
__m128 _mm_mul_ps(__m128 a, __m128 b);
__m128 _mm_shuffle_ps(__m128 a, __m128 b, unsigned int imm);
__m128 _mm_add_ss(__m128 a, __m128 b);
__m128 _mm_sqrt_ss(__m128 a);
}

#define VISIT_SHUFFLE(z, y, x, w) (((z) << 6) | ((y) << 4) | ((x) << 2) | (w))

class Object00471790 {
public:
    float field_00[4];   // vector x/y/z (+w padding read by the load)

    float sub_00471790();
};

// sub_00471f90: __thiscall member, two stack
// parameters (RET 8): a destination pointer and a byte count.
//
// Writer of a cursor/serialization object: +0x04 is the current read
// position. Copies `count` bytes from the cursor to the destination with the
// MSVC 7.1 memcpy intrinsic (REP MOVSD dword run, then REP MOVSB tail), then
// advances the cursor by the byte count. The cursor is re-read from memory
// for the advance, so the copy leaves no redundant reload. Callers are the
// sub_004968e0 reader at 0x00496963 and 0x004969b8.

extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)

class Object00471f90 {
public:
    unsigned char unknown_00[0x4];
    unsigned char* field_04;

    void sub_00471f90(void* destination, unsigned int count);
};

typedef char Object00471f90_size[(sizeof(Object00471f90) == 8) ? 1 : -1];

// sub_004728c0: __thiscall with two stack words
// (RET 8): a record cursor and a type-filter flag. Compiled with MSVC 7.1.
//
// Container of 0xc-byte records: dword count at +0, record base at +4; each
// record holds a type dword at +0. Scans forward from the record strictly
// after `cursor` up to and including the last record (base + count - 1); the
// first record whose type is outside [0x15, 0x17] — or any record when the
// incoming flag is zero — is returned. Returns 0 for a null cursor, when the
// cursor already sits on the last record, or at the end of the chain.
//
// Callers reach it through the thunks at 0x004737f0 and 0x00474410/0x004744f0/
// 0x00474850, which pass the container at outer_this + 0x10 + 0x20 with the
// previous scan result as the cursor. Names are hypotheses.
//
// The do/while (flag) tail is load-bearing: flag was just set to 1, so the
// condition folds away and MSVC keeps the leading exit test at the loop head
// with a plain JMP back edge. A while (cursor != last) loop gets inverted
// into an entry test plus a duplicated bottom compare.
#include "layout.h"

struct Record004728c0 {
    unsigned int type;
    unsigned char unknown_04[8];
};

struct Object004728c0 {
    unsigned int count;             // +0
    Record004728c0* base;           // +4
    unsigned char unknown_08[4];

    Record004728c0* sub_004728c0(Record004728c0* cursor, int flag);
};

LAYOUT_OFFSET(Object004728c0, base, 0x4);
LAYOUT_SIZE(Record004728c0, 0xc);

// sub_00472900: __thiscall with a float and an int
// stack word (RET 8). Compiled with MSVC 7.1.
//
// Container of 0xc-byte records: dword count at +0, record base at +4; each
// record holds a type dword at +0 and a float at +8. Scans from the last
// record (index count - 1) down to index 0 and returns the first record whose
// float field is less than `value` and whose type is outside [0x15, 0x17]
// (any type when `flag` is zero). Returns 0 when the container is empty or
// nothing matches. Reached through the thunk at 0x00473810, which passes the
// container at outer_this + 0x10 + 0x20. Names are hypotheses.
#include "layout.h"

struct Record00472900 {
    unsigned int type;
    unsigned char unknown_04[4];
    float field_08;
};

struct Object00472900 {
    unsigned int count;             // +0
    Record00472900* base;           // +4
    unsigned char unknown_08[4];

    Record00472900* sub_00472900(float value, int flag);
};

LAYOUT_OFFSET(Object00472900, base, 0x4);
LAYOUT_OFFSET(Record00472900, field_08, 0x8);
LAYOUT_SIZE(Record00472900, 0xc);

// sub_00472950: __thiscall with a float and an int
// stack word (RET 8). Compiled with MSVC 7.1.
//
// Byte-identical twin of sub_00472900 at a different address; no EXE call
// site reaches it directly, so it most likely sits in an as-yet-unmapped
// dispatch table. Container of 0xc-byte records (count at +0, base at +4,
// type dword at +0, float at +8). Scans from the last record down to index 0
// and returns the first record whose float field is less than `value` and
// whose type is outside [0x15, 0x17] (any type when `flag` is zero).
// Returns 0 when the container is empty or nothing matches. Names are
// hypotheses.
#include "layout.h"

struct Record00472950 {
    unsigned int type;
    unsigned char unknown_04[4];
    float field_08;
};

struct Object00472950 {
    unsigned int count;             // +0
    Record00472950* base;           // +4
    unsigned char unknown_08[4];

    Record00472950* sub_00472950(float value, int flag);
};

LAYOUT_OFFSET(Object00472950, base, 0x4);
LAYOUT_OFFSET(Record00472950, field_08, 0x8);
LAYOUT_SIZE(Record00472950, 0xc);

// sub_004729a0: __thiscall with two stack words
// (RET 8): a record cursor and a type-filter flag. Compiled with MSVC 7.1.
//
// Container of 0xc-byte records: record base at +4 (the count at +0 is not
// read); each record holds a type dword at +0. Scans backwards from `cursor`
// while the cursor stays above `base`; when the current record's type is
// outside [0x15, 0x17] — or for any record when the incoming flag is zero —
// returns the record one step before the cursor. Returns 0 for a null
// cursor or when the scan reaches the base. Reached through the thunk at
// 0x00473830, which passes the container at outer_this + 0x10 + 0x20.
// Names are hypotheses.
#include "layout.h"

struct Record004729a0 {
    unsigned int type;
    unsigned char unknown_04[8];
};

struct Object004729a0 {
    unsigned char unknown_00[4];
    Record004729a0* base;           // +4
    unsigned char unknown_08[4];

    Record004729a0* sub_004729a0(Record004729a0* cursor, int flag);
};

LAYOUT_OFFSET(Object004729a0, base, 0x4);
LAYOUT_SIZE(Record004729a0, 0xc);

// sub_004729e0: __thiscall with no stack words.
// Compiled with MSVC 7.1.
//
// Clears the dword at +0x10 and returns the receiver in EAX
// (MOV EAX,ECX precedes the store). No EXE references reach it; it is most
// likely a table entry. Field purpose unknown. Names are hypotheses.
#include "layout.h"

struct Object004729e0 {
    unsigned char unknown_00[0x10];
    unsigned int field_10;

    Object004729e0* sub_004729e0();
};

LAYOUT_OFFSET(Object004729e0, field_10, 0x10);

// sub_004729f0: __thiscall with no stack words,
// returns a dword. Compiled with MSVC 7.1.
//
// Maps the dword at +0x4 through an unsigned range chain: values 0xfffffff0
// and 0xfffffff1 map to 0xfffffff0, the value 0xfffffff2 is returned
// unchanged (the fall-through RET keeps the loaded field), and everything
// else maps to 0. Callers at 0x004836a3, 0x004867fc, 0x004ab602, 0x004ce1df
// and 0x005ce236. Field purpose unknown; the value looks like a two's-
// complement enum. Names are hypotheses.
#include "layout.h"

struct Object004729f0 {
    unsigned char unknown_00[4];
    unsigned int field_04;

    unsigned int sub_004729f0();
};

LAYOUT_OFFSET(Object004729f0, field_04, 0x4);

#include "layout.h"
// Object004734d0: one object, reached through the global at 0x007ca200.
//
// Merge evidence: EXE code loads the same global as `this` for sub_004734d0
// (0x0056fbf2) and for sub_004736b0 (0x00531861, 0x005318ea, 0x0049439d,
// 0x005d4a7d). The field offsets do not overlap. Names are hypotheses.
// sub_004739a0, which the same global also feeds, is a free function.
struct Object004734d0 {
    unsigned char unknown_00[0x3c];
    unsigned int field_3c;
    unsigned char unknown_40[0xa8];
    unsigned int field_e8;

    void sub_004734d0();
    unsigned int sub_004736b0(unsigned int value);
};
LAYOUT_OFFSET(Object004734d0, field_3c, 0x3c);
LAYOUT_OFFSET(Object004734d0, field_e8, 0xe8);

// sub_00473e70: __thiscall with four stack words
// (RET 0x10). Compiled with MSVC 7.1.
//
// The receiver is an array of four 0x10-byte slots at +0: three payload
// dwords at +0/+4/+8 and a float at +0xc used as the occupancy key. Finds
// the first slot whose key equals the zero float at 0x00776a70, fills all
// four fields (the fourth argument lands in the key) and returns 1; returns
// 0 when every slot is occupied. The sub_004767a0 dispatcher calls it on the
// slot array at outer receiver + 0x100 after swapping its third and fourth
// arguments. Names are hypotheses.
#include "layout.h"

struct Slot00473e70 {
    unsigned int field_0;
    unsigned int field_4;
    unsigned int field_8;
    float key;                      // +0xc, compared with 0.0f
};

struct Object00473e70 {
    Slot00473e70 slots[4];

    int sub_00473e70(unsigned int value0, unsigned int value1,
                     unsigned int value2, float key);
};

LAYOUT_OFFSET(Slot00473e70, key, 0xc);
LAYOUT_SIZE(Slot00473e70, 0x10);
LAYOUT_SIZE(Object00473e70, 0x40);

#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
// The entry returns this in EAX while it clears three fields.
struct Object00473fd0 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    Object00473fd0();
};
typedef char offset_check[offsetof(Object00473fd0, field_08) == 8 ? 1 : -1];

// sub_00474950: __thiscall with no stack words,
// returns 0/1. Compiled with MSVC 7.1.
//
// State check on the object fed by the global at 0x007ca200. Guards: the
// pointer at +0x10 must be set, the state dword at +0xe8 must equal 1, the
// key float of slot 0 (+0x10c) must equal the zero float at 0x00776a70, and
// the flag bits 0-1 of the byte at +0xf8 must be clear; otherwise returns 1.
// The tail compares the floats at +0xfc, +0xec, +0xf4 and +0xf0 against the
// same zero constant. The vtable data reference at 0x0070f988 marks this as
// a virtual method; the definition here is the direct body. Callers:
// 0x004767f5, 0x0056fb7d, 0x0054eb99, 0x005a1eb4, 0x0047fcf4.
// Field purposes unknown. Names are hypotheses.
//
// The negated `!(field_ec >= field_f4)` is load-bearing: it selects the
// TEST AH,1 / JNE encoding (unordered counts as less) and keeps the shared
// return-0 block as the fall-through; a plain `<` compiles to TEST AH,5/JNP.
#include "layout.h"

struct Object00474950 {
    unsigned char unknown_00[0x10];
    void* field_10;                 // +0x10, tested for zero
    unsigned char unknown_14[0xd4];
    unsigned int field_e8;          // +0xe8, compared with 1
    float field_ec;
    float field_f0;
    float field_f4;
    unsigned char field_f8;         // +0xf8, flag bits 0-1 tested
    unsigned char unknown_f9[3];
    float field_fc;
    unsigned char unknown_100[0xc];
    float field_10c;                // +0x10c, slot 0 key

    int sub_00474950();
};

LAYOUT_OFFSET(Object00474950, field_10, 0x10);
LAYOUT_OFFSET(Object00474950, field_e8, 0xe8);
LAYOUT_OFFSET(Object00474950, field_ec, 0xec);
LAYOUT_OFFSET(Object00474950, field_f8, 0xf8);
LAYOUT_OFFSET(Object00474950, field_fc, 0xfc);
LAYOUT_OFFSET(Object00474950, field_10c, 0x10c);

// NONMATCHING
// FUNCTION: RBR 0x00471110
extern "C" int __stdcall sub_00471110(void* instance, void* previousInstance,
                                      char* commandLine, int showCommand) {
    bool runIntro = false;
    if (gGuard007ca1f0 == 0xb7) { // startup guard: main already ran
        return 0;
    }
    gLogToStdout = 0;
    SetUnhandledExceptionFilter((void*)sub_00470f70);

    Object0040f9f0 iniReader;
    iniReader.sub_0040f9f0();
    iniReader.sub_0040fb60(std::string("RichardBurnsRally.ini"), 2); // "RichardBurnsRally.ini"

    runIntro = iniReader.sub_004100e0( // section "Settings", key "Fullscreen", default on
                       std::string("Settings"), std::string("Fullscreen"), 1)
            && iniReader.sub_004100e0( // section "Settings", key "RunIntro", default on
                       std::string("Settings"), std::string("RunIntro"), 1);

    if (iniReader.sub_004100e0( // section "Settings", key "RunDiagnose", default off
            std::string("Settings"), std::string("RunDiagnose"), 0)) {
        sub_0041c8a0(0);
    }
    bool runStartup = iniReader.sub_004100e0( // section "Settings", key "RunStartup", default on
            std::string("Settings"), std::string("RunStartup"), 1);
    if (runStartup) {
        if (sub_00470f00()) {
            iniReader.sub_0040fa60();
            return 0;
        }
    }
    sub_00470ca0(runIntro, instance);

    if (iniReader.sub_004100e0( // section "Settings", key "RunIntro" again after the startup run
            std::string("Settings"), std::string("RunIntro"), 1)) {
        sub_00470ea0();
        sub_00470e60();
        sub_00470ca0(runIntro, instance);
    }

    // MSVC 7.1 has no __cpuid intrinsic, so the original needed inline assembly.
    unsigned int features;
    __asm {
        mov eax, 1
        cpuid
        mov features, edx
    }
    if ((features & 0x02000000) == 0) { // EDX bit 25: SSE
        MessageBoxA(0, "You are running the wrong executeable!\r\nThe processor you are running does not support the SSE instruction set.\r\nPlease run Rally7App_3DNow.exe instead.", "SSE not supported", 0x10);
        iniReader.sub_0040fa60();
        return 0;
    }

    if (CoInitialize(0) < 0) {
        RESTORE_CURSOR_AND_WINDOW()
        MessageBoxA(0, "Could not initialize COM!", "Fatal error", 0);
        iniReader.sub_0040fa60();
        return 1;
    }

    GamePlatform0044a3c0*& platform = gPlatform;
    void* platformMemory = sub_006efa32(0x1bd0);
    GamePlatform0044a3c0* created = 0;
    if (platformMemory != 0) {
        created = ((GamePlatform0044a3c0*)platformMemory)->sub_0044a3c0(instance);
    }
    platform = created;

    const char* commandLineEnd = commandLine;
    while (*commandLineEnd != '\0') ++commandLineEnd;
    if (commandLineEnd != commandLine) {
        platform->sub_00446bd0(std::string(commandLine));
    }

    if (platform->sub_0047e860() == 0) {
        sub_0043b3c0(7, 0, "Error in PrePlatformInit"); // "Error in PrePlatformInit"
    }
    if (platform->platformInit() == 0) {
        sub_0043b3c0(7, 0, "Error in PlatformInit"); // "Error in PlatformInit"
    }

    AppWindow007c9fe8*& application = gApplication;
    SetForegroundWindow((void*)application->window->sub_0040e570());
    SetFocus((void*)application->window->sub_0040e570());
    RESTORE_CURSOR_AND_WINDOW()

    if (platform->sub_0047eb00() == 0) {
        sub_0043b3c0(7, 0, "Error in PostPlatformInit"); // "Error in PostPlatformInit"
    }
    platform->shutdown();
    platform->sub_00479e00();
    platform->run();
    platform->sub_004807c0();
    delete platform;
    platform = 0;
    CoUninitialize();
    iniReader.sub_0040fa60();
    return 0;
}

// FUNCTION: RBR 0x004716c0
void Object004716c0::sub_004716c0() { field_04 = 0; }

// NONMATCHING
// FUNCTION: RBR 0x00471790
float Object00471790::sub_00471790() {
    const float *volatile v = field_00;
    __m128 rot = _mm_loadu_ps(v);
    rot = _mm_mul_ps(rot, rot);
    __m128 sum = rot;
    rot = _mm_shuffle_ps(rot, rot, VISIT_SHUFFLE(2, 1, 2, 1));
    __m128 sum2 = _mm_add_ss(sum, rot);
    rot = _mm_shuffle_ps(rot, rot, VISIT_SHUFFLE(2, 1, 2, 1));
    __m128 sum3 = _mm_add_ss(sum2, rot);
    return _mm_sqrt_ss(sum3).m128_f32[0];
}

// FUNCTION: RBR 0x00471f90
void Object00471f90::sub_00471f90(void* destination, unsigned int count) {
    memcpy(destination, field_04, count);
    field_04 += count;
}

// FUNCTION: RBR 0x004728c0
Record004728c0* Object004728c0::sub_004728c0(Record004728c0* cursor, int flag) {
    if (cursor != 0) {
        do {
            if (cursor == base + count - 1)
                return 0;
            ++cursor;
            if (flag == 0 || cursor->type < 0x15 || cursor->type > 0x17)
                return cursor;
            flag = 1;
        } while (flag);
    }
    return 0;
}

// FUNCTION: RBR 0x00472900
Record00472900* Object00472900::sub_00472900(float value, int flag) {
    int i = count - 1;
    if (i >= 0) {
        do {
            if (base[i].field_08 < value
                && (flag == 0 || base[i].type < 0x15 || base[i].type > 0x17))
                return base + i;
        } while (--i >= 0);
    }
    return 0;
}

// FUNCTION: RBR 0x00472950
Record00472950* Object00472950::sub_00472950(float value, int flag) {
    int i = count - 1;
    if (i >= 0) {
        do {
            if (base[i].field_08 < value
                && (flag == 0 || base[i].type < 0x15 || base[i].type > 0x17))
                return base + i;
        } while (--i >= 0);
    }
    return 0;
}

// FUNCTION: RBR 0x004729a0
Record004729a0* Object004729a0::sub_004729a0(Record004729a0* cursor, int flag) {
    if (cursor != 0) {
        do {
            if (cursor == base)
                return 0;
            if (flag == 0 || cursor->type < 0x15 || cursor->type > 0x17)
                return cursor - 1;
            --cursor;
            flag = 1;
        } while (cursor != 0);
    }
    return 0;
}

// FUNCTION: RBR 0x004729e0
Object004729e0* Object004729e0::sub_004729e0() {
    field_10 = 0;
    return this;
}

// FUNCTION: RBR 0x004729f0
unsigned int Object004729f0::sub_004729f0() {
    unsigned int value = field_04;
    switch (value) {
    case 0xfffffff0:
    case 0xfffffff1:
        return 0xfffffff0u;
    case 0xfffffff2:
        return value;
    }
    return 0;
}

// FUNCTION: RBR 0x004734d0
void Object004734d0::sub_004734d0() { field_e8 = 0; }

// FUNCTION: RBR 0x004736b0
unsigned int Object004734d0::sub_004736b0(unsigned int value) { return field_3c + value; }

// FUNCTION: RBR 0x004739a0
extern "C" unsigned int sub_004739a0() { return 0; }

// FUNCTION: RBR 0x00473e70
int Object00473e70::sub_00473e70(unsigned int value0, unsigned int value1,
                                 unsigned int value2, float key) {
    for (unsigned int i = 0; i < 4; i++) {
        if (slots[i].key == 0.0f) {
            slots[i].field_0 = value0;
            slots[i].field_4 = value1;
            slots[i].field_8 = value2;
            slots[i].key = key;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: RBR 0x00473fd0
Object00473fd0::Object00473fd0() { field_00 = 0; field_04 = 0; field_08 = 0; }

// FUNCTION: RBR 0x00474950
int Object00474950::sub_00474950() {
    if (field_10 == 0)
        return 0;
    if (field_e8 != 1)
        return 1;
    if (field_10c != 0.0f)
        return 1;
    if (field_f8 & 3)
        return 1;
    if (field_fc > 0.0f) {
        if (!(field_ec >= field_f4))
            return 1;
        return 0;
    }
    if (field_ec <= field_f0)
        return 0;
    return 1;
}
