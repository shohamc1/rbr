// sub_00402d90: EXE 0x00402d90, 46 bytes. MSVC 7.1 (/O2 /Zl).
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
