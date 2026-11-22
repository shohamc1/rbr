// sub_00402a00: MSVC 7.1 (/O2 /Zl).
//
// Destructor thunk of the SSO string class whose deleting destructor is
// sub_00402980 (vtable 0x0070c79c; this is the body the deleting destructor
// calls at 0x00402983). Stores the class vtable constant 0x0070c79c into
// this+0 and tail-jumps into the base-class destructor body sub_004029a0
// (EXE 0x004029a0), which stores vtable 0x0070c790, destroys the
// basic_string at +0xc through the MSVCP71 import at IAT 0x0070c2d8 and the
// sub-object through IAT 0x0070c4f8 inside an SEH frame. __thiscall member
// of Object00402980 (shared class shape with Unit004021e0.cpp); ECX passes
// through unchanged, EAX/flags are whatever the callee leaves.

extern "C" const void* const gVtable0070c79c[]; // EXE .rdata 0x0070c79c

class Object004029a0 {
public:
    void sub_004029a0();   // base-class destructor body, EXE 0x004029a0
};

class Object00402980 {
public:
    void* field_00;   // vtable slot; 0x0070c79c for this class
    void sub_00402a00();
};

// sub_00402b50: MSVC 7.1 (/O2 /Zl).
//
// Streams a float triple to std::ostream as "(x, y, z)":
//   stream << "(" << x << ", " << y << ", " << z << ")";
// __cdecl with the ostream reference and three by-value floats; result
// discarded, bare RET.
//
// MSVCP71 imports (see msvcp71.h for the mangling rules: char_traits must be
// a struct): the free template operator<<<char_traits<char> > for const char*
// through IAT 0x0070c344, folded into one load and four CALL ESI sites, and
// the thiscall member basic_ostream::operator<<(float) through IAT 0x0070c274.
// The strings are the EXE .rdata literals, addressed by their pinned
// addresses ("(" 0x0076f478, ", " 0x0076f474, ")" 0x0076f480) so the object
// defines no local data.

namespace std {
    template<class Element> struct char_traits;

    template<class Element, class Traits>
    class basic_ostream {
    public:
        __declspec(dllimport) basic_ostream& operator<<(float value);
    };

    template<class Traits>
    __declspec(dllimport)
    basic_ostream<char, Traits>& __cdecl operator<<(basic_ostream<char, Traits>& stream,
                                                    const char* text);
}

// NONMATCHING (7 instructions, 89 of 96 bytes): under /O2 the compiler folds
// the basic_ostream::operator<<(float) import into EDI (push/pop EDI,
// "mov edi,[0x70c274]" once, three "call edi"), while the reference keeps
// three "call dword ptr [0x70c274]" sites; the mid-run stream reload shifts
// from [esp+0x10] to [esp+0x14] accordingly. The free operator<< import folds
// into ESI in both. Tried without improvement: split statements (loses the
// argument push-run), explicit operator<< call syntax, a local function
// pointer initialized from the dllimport address, __declspec(dllimport) on
// the class (disables the push-run and the ESI fold; adds an EBP frame),
// #pragma optimize("g"/"t"), and /G5, /G7, /Oi.

// sub_00402c20: MSVC 7.1 (/O2 /Zl).
//
// Character sink of the log/stream class referenced from the record at
// 0x0076f438: flag bit 1 at +0x40 mirrors to OutputDebugStringA, flag bit 0
// writes through the FILE* at +0x3c with fprintf("%c") followed by fflush.
// The -1 input is the "no character" sentinel and returns unchanged. The
// two-byte string for OutputDebugStringA is built in the incoming argument
// slot. thiscall, one stack argument, returns the character in EAX.
//
// Imports: KERNEL32 OutputDebugStringA (IAT 0x0070c1c0, stdcall), MSVCR71
// fprintf (0x0070c59c) and fflush (0x0070c5d0), cdecl. The single
// "add esp,0x10" cleans the three fprintf pushes and the fflush push.

extern "C" {
__declspec(dllimport) void __stdcall OutputDebugStringA(const char* text);
__declspec(dllimport) int __cdecl fprintf(void* stream, const char* format, ...);
__declspec(dllimport) int __cdecl fflush(void* stream);
}

struct Object00402c20 {
    unsigned char field_00[0x3c];
    void* field_3c;            // FILE* sink
    unsigned char field_40;    // mirror flags: bit 0 file, bit 1 debug string
    int sub_00402c20(int character);
};

// sub_00402c80: MSVC 7.1 (/O2 /Zl).
//
// String sink of the log/stream class referenced from the record at
// 0x0076f450 (same receiver layout as sub_00402c20): flag bit 1 at +0x40
// mirrors to OutputDebugStringA, flag bit 0 writes through the FILE* at
// +0x3c with fprintf("%s") followed by fflush. Returns its second argument
// (the byte count) unchanged, like a streambuf write-back. thiscall, two
// stack arguments, RET 8.
//
// Imports: KERNEL32 OutputDebugStringA (IAT 0x0070c1c0, stdcall), MSVCR71
// fprintf (0x0070c59c) and fflush (0x0070c5d0), cdecl; the single
// "add esp,0x10" cleans the three fprintf pushes and the fflush push.

extern "C" {
__declspec(dllimport) void __stdcall OutputDebugStringA(const char* text);
__declspec(dllimport) int __cdecl fprintf(void* stream, const char* format, ...);
__declspec(dllimport) int __cdecl fflush(void* stream);
}

struct Object00402c80 {
    unsigned char field_00[0x3c];
    void* field_3c;            // FILE* sink
    unsigned char field_40;    // mirror flags: bit 0 file, bit 1 debug string
    int sub_00402c80(const char* text, int length);
};

// sub_00402d90: MSVC 7.1 (/O2 /Zl).
//
// Scalar deleting destructor (MSVC ??_G shape) referenced from vtable
// 0x0076f42c, the vtable stored at full+0x70 of a stream object whose first
// 0x70 bytes are a non-polymorphic ios_base-like head (virtual-base stream
// layout). The deleting destructor therefore receives the vtable-owning
// subobject at full+0x70 in ECX, adjusts to the full object (LEA ESI,
// [ECX-0x70]), recomputes the subobject pointer from it (LEA EDI,[ESI+0x70]),
// runs the complete finalizer body sub_00402de0 on the subobject, then the
// imported std::basic_ios<char>::~basic_ios destructor (IAT 0x0070c2fc) on
// the same subobject, frees the full object through the cdecl operator-delete
// thunk sub_006efdd0 when bit 0 of the flags argument is set, and returns the
// full object in EAX. RET 4 pops the flags.

namespace std {
    template<class Element> struct char_traits;

    template<class Element, class Traits>
    class basic_ios {
    public:
        __declspec(dllimport) virtual ~basic_ios();
    };
}

// 6-byte cdecl operator-delete thunk at EXE 0x006efdd0 (jmp [0x0070c504]).
extern "C" void __cdecl sub_006efdd0(void* memory);

class Object00402d90 : public std::basic_ios<char, std::char_traits<char> > {
public:
    void sub_00402de0();   // complete finalizer body, EXE 0x00402de0 (SEH)
    void* sub_00402d90(unsigned int flags);
};

struct Full00402d90 {
    unsigned char field_00[0x70];   // non-polymorphic head (ios_base-like)
    Object00402d90 field_70;        // vtable-owning stream subobject
};

// FUNCTION: RBR 0x00402a00
void Object00402980::sub_00402a00() {
    field_00 = (void*)gVtable0070c79c;
    ((Object004029a0*)this)->sub_004029a0();
}

// NONMATCHING
// FUNCTION: RBR 0x00402b50
extern "C" void __cdecl sub_00402b50(std::basic_ostream<char, std::char_traits<char> >& stream,
                                     float x, float y, float z) {
    stream << "(" << x
           << ", " << y
           << ", " << z
           << ")";
}

// FUNCTION: RBR 0x00402c20
int Object00402c20::sub_00402c20(int character) {
    if (character != -1) {
        if (field_40 & 2) {
            char text[2];
            text[0] = (char)character;
            text[1] = 0;
            OutputDebugStringA(text);
        }
        if ((field_40 & 1) && field_3c != 0) {
            fprintf(field_3c, "%c", character);
            fflush(field_3c);
        }
    }
    return character;
}

// FUNCTION: RBR 0x00402c80
int Object00402c80::sub_00402c80(const char* text, int length) {
    if (field_40 & 2) {
        OutputDebugStringA(text);
    }
    if ((field_40 & 1) && field_3c != 0) {
        fprintf(field_3c, "%s", text);
        fflush(field_3c);
    }
    return length;
}

// FUNCTION: RBR 0x00402d90
void* Object00402d90::sub_00402d90(unsigned int flags) {
    Full00402d90* owner = (Full00402d90*)((char*)this - 0x70);
    Object00402d90* stream = &owner->field_70;
    stream->sub_00402de0();
    stream->std::basic_ios<char, std::char_traits<char> >::~basic_ios();
    if ((flags & 1) != 0) {
        sub_006efdd0(owner);
    }
    return owner;
}
