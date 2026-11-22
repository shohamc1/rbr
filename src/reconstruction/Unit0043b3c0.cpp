// Logging path: the error wrapper sub_0043b3c0 forwards its message and
// variadic argument pointer to the logging core sub_0048cc00, which writes to
// stdout or to a sink object. Compiled with MSVC 7.1 (/O2 /Zl).
//
// stdarg.h is unavailable: the pinned MSVC 7.1 Docker image mounts no include
// path. The core takes a plain char* argument pointer.

extern "C" void __cdecl sub_0048cc00(int kind, int code, const char *message,
                                     char *arguments);

// Error-reporting wrapper: forwards a formatted-message call to the logging
// core at sub_0048cc00 (cdecl, four stack arguments, the last being the
// variadic argument pointer). Callers pass (7, 0, message) with no variadic
// arguments, e.g. "Error in PlatformInit".
//
// The variadic argument pointer is the MSVC x86 va_start expansion
// (ap = (va_list)&lastParam + 4).

// sub_0043bfe0: Compiled with MSVC 7.1 (/O2 /Zl).
//
// thiscall notification forwarder. If the object at +0xa0 exists, takes the
// receiver from the application global at 0x007c9fe8 (field +0x40) and
// calls virtual slot 0x58/4 = 22 on it with (this->field_a0->field_70, 0).
// The callee cleans both stack words (caller does no ADD ESP), so the slot
// is a thiscall member with two stack arguments. The dispatch shape
// (MOV ECX,[app]; MOV ECX,[ECX+0x40]; MOV EDX,[ECX]; CALL [EDX+0x58]) and
// the application-global identity are from the reference bytes; names and
// the receiver class are hypotheses. No EXE caller is known yet.
#include "layout.h"

// Receiver reached through the application global 0x007c9fe8 at +0x40. Only
// slot 22 (offset 0x58) is evidenced; the other entries are placeholders so
// the slot lands at the right vtable offset.
class Object0043bfReceiver {
public:
    virtual void slot_00();
    virtual void slot_04();
    virtual void slot_08();
    virtual void slot_0c();
    virtual void slot_10();
    virtual void slot_14();
    virtual void slot_18();
    virtual void slot_1c();
    virtual void slot_20();
    virtual void slot_24();
    virtual void slot_28();
    virtual void slot_2c();
    virtual void slot_30();
    virtual void slot_34();
    virtual void slot_38();
    virtual void slot_3c();
    virtual void slot_40();
    virtual void slot_44();
    virtual void slot_48();
    virtual void slot_4c();
    virtual void slot_50();
    virtual void slot_54();
    virtual void slot_58(void* value, int zero);
};

struct App007c9fe8 {
    unsigned char unknown_00[0x40];
    Object0043bfReceiver* field_40;
};

LAYOUT_OFFSET(App007c9fe8, field_40, 0x40);

// Object referenced at +0xa0; its dword at +0x70 becomes the first argument.
struct Target0043bfe0 {
    unsigned char unknown_00[0x70];
    void* field_70;
};

LAYOUT_OFFSET(Target0043bfe0, field_70, 0x70);

extern "C" App007c9fe8* gApplication; // EXE .data 0x007c9fe8

class Object0043bfe0 {
public:
    void sub_0043bfe0();
    unsigned char unknown_00[0xa0];
    Target0043bfe0* field_a0;
};

LAYOUT_OFFSET(Object0043bfe0, field_a0, 0xa0);

// FUNCTION: RBR 0x0043b3c0
extern "C" void __cdecl sub_0043b3c0(int kind, int code, const char *message,
                                     ...) {
    char *arguments = reinterpret_cast<char *>(&message) + 4;
    sub_0048cc00(kind, code, message, arguments);
}

// FUNCTION: RBR 0x0043bfe0
void Object0043bfe0::sub_0043bfe0() {
    Target0043bfe0* target = field_a0;
    if (target != 0) {
        gApplication->field_40
            ->slot_58(target->field_70, 0);
    }
}
