// sub_00402970: EXE 0x00402970, 14 bytes. MSVC 7.1 (/O2 /Zl).
//
// SSO string buffer getter (c_str style) for the class whose vtable is
// 0x0070c79c: slot 0 is the deleting destructor sub_00402980 and this is
// slot 1. The receiver embeds a 0x10-byte prefix, then an MSVCP71-shaped
// string body: SSO buffer/pointer union at +0x10, length at +0x20, capacity
// at +0x24 (compare msvcp71.h, where the same body sits at offset 0 of
// std::string). Capacity < 0x10 selects the inline buffer at +0x10; otherwise
// the heap pointer stored at +0x10. thiscall, no stack arguments, returns
// the character pointer in EAX.

struct Object00402970 {
    unsigned char field_00[0x10];
    union {
        char field_10_buffer[0x10];
        char* field_10_pointer;
    };
    unsigned int field_20;
    unsigned int field_24;
    const char* sub_00402970() const;
};

const char* Object00402970::sub_00402970() const {
    if (field_24 >= 0x10) {
        return field_10_pointer;
    }
    return field_10_buffer;
}
