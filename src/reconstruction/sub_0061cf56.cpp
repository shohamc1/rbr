// sub_0061cf56: EXE 0x0061cf56, 19 bytes. MSVC 7.1 (/O2 /Zl).
//
// Zero-initializer member returning this: clears the five dwords at
// +0x00..+0x10 and returns the receiver in EAX (ECX-in/EAX-out shape).
// Callers (0x00612576, 0x00610227, 0x0061030e, 0x00610bbf, 0x00610dd5)
// pass the object in ECX; no stack arguments (bare RET).

class Object0061cf56 {
public:
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
    unsigned int field_10;

    Object0061cf56* sub_0061cf56();
};

Object0061cf56* Object0061cf56::sub_0061cf56() {
    field_00 = 0;
    field_04 = 0;
    field_08 = 0;
    field_0c = 0;
    field_10 = 0;
    return this;
}
