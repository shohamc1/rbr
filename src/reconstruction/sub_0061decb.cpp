// sub_0061decb: EXE 0x0061decb, 85 bytes. MSVC 7.1 (/O2 /Zl /Oy-).
//
// Color unpack member returning this: writes the four bytes of the packed
// color argument to *this as floats scaled by 1/255 (gColorScale0070e9ec)
// (byte 2 first, then byte 1, byte 0, byte 3), each extracted with MOVZX
// from an (unsigned char) cast (so the FILD dword conversion is signed and
// needs no 2^32 fixup) through the reused argument slot at [EBP+8], and
// multiplied by the scale held in the x87 register. The /Oy- frame keeps
// the reference's EBP addressing; the return-this homing is what puts this
// in EAX and the packed word in ECX. __thiscall with one stack word
// (RET 4).

// A literal 1.0f / 255.0f makes MSVC memory-form every FMUL; the reference
// loads this .rdata float once and keeps it in ST(1), as for a named constant.
extern "C" const float gColorScale0070e9ec; // 1/255, EXE .rdata 0x0070e9ec

class Object0061decb {
public:
    float field_00[4];

    Object0061decb* sub_0061decb(unsigned int packed);
};

Object0061decb* Object0061decb::sub_0061decb(unsigned int packed) {
    float scale = gColorScale0070e9ec;
    field_00[0] = (float)(unsigned char)(packed >> 16) * scale;
    field_00[1] = (float)(unsigned char)(packed >> 8) * scale;
    field_00[2] = (float)(unsigned char)packed * scale;
    field_00[3] = (float)(unsigned char)(packed >> 24) * scale;
    return this;
}
