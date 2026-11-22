#define offsetof(type, member) ((unsigned int)&(((type *)0)->member))
// The entry returns this in EAX while it clears four fields.
struct Object0048cb70 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
    Object0048cb70();
};
typedef char offset_check[offsetof(Object0048cb70, field_0c) == 0xc ? 1 : -1];

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

// sub_0048cc90: Base finalizer of the sub_0044cfe0
// class family near the logging core sub_0048cc00. It installs the vtable
// 0x00712b90 into the object at hand and returns. The vtable's slot 0 holds
// the deleting finalizer sub_0048cd50; slot 1 repeats the shared stub
// 0x006eff06 that the 0x004491e0-family vtables also use. Compiled with
// MSVC 7.1 (/O2 /Zl). Raw-address store: no object-local vtable is emitted.

extern "C" const void* const gVtable00712b90[]; // EXE .rdata 0x00712b90

class Object0048cc90 {
public:
    unsigned int field_00;   // vtable slot; 0x00712b90 when constructed
    void sub_0048cc90();
};

// FUNCTION: RBR 0x0048cb70
Object0048cb70::Object0048cb70() { field_08 = 0; field_00 = 0; field_04 = 0; field_0c = 0; }

// Logging core reached from the error wrapper sub_0043b3c0 as
// (kind, code, message, argument pointer). Messages go to stdout when the
// 0x007a18f8 flag is set. Otherwise (code * 8 + kind) indexes a dword table at
// 0x007a29d8, whose value selects an 8-byte record in the array at 0x007a5ab4.
// The first field of a record is a sink object. The message is formatted into
// the shared buffer at 0x007cc4b0 and the sink receives (buffer, length)
// through virtual slot 1. Names are hypotheses from this function alone.
//
// MSVCR71 imports (IAT 0x0070c5ac vprintf, 0x0070c580 vsprintf).
extern "C" {
__declspec(dllimport) int __cdecl vprintf(const char *format, char *arguments);
__declspec(dllimport) int __cdecl vsprintf(char *buffer, const char *format,
                                           char *arguments);
}

// Slot 0 is unidentified. Slot 1 consumes the formatted line.
struct LogSink {
    virtual void Reserved0();
    virtual void Write(const char *text, int length);
};

struct LogRecord {
    LogSink *sink;
    int field_4;
};
typedef char AssertLogRecordSize[sizeof(LogRecord) == 8 ? 1 : -1];

extern "C" {
extern int gLogToStdout;           // 0x007a18f8
extern int gLogRouteTable[];       // 0x007a29d8, index = code * 8 + kind
extern LogRecord gLogRecords[];    // 0x007a5ab4
extern char gLogLineBuffer[];      // 0x007cc4b0
}

// FUNCTION: RBR 0x0048cc00
extern "C" void __cdecl sub_0048cc00(int kind, int code, const char *message,
                                     char *arguments) {
    if (gLogToStdout) {
        vprintf(message, arguments);
        return;
    }
    LogSink *sink = gLogRecords[gLogRouteTable[code * 8 + kind]].sink;
    if (sink) {
        int length = vsprintf(gLogLineBuffer, message, arguments);
        sink->Write(gLogLineBuffer, length);
    }
}

// FUNCTION: RBR 0x0048cc90
void Object0048cc90::sub_0048cc90() {
    field_00 = (unsigned int)gVtable00712b90;
}
