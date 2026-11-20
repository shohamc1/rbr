// Logging core reached from the error wrapper sub_0043b3c0 as
// (kind, code, message, argument pointer). Messages go to stdout when the
// 0x007a18f8 flag is set. Otherwise (code * 8 + kind) indexes a dword table at
// 0x007a29d8, whose value selects an 8-byte record in the array at 0x007a5ab4.
// The first field of a record is a sink object. The message is formatted into
// the shared buffer at 0x007cc4b0 and the sink receives (buffer, length)
// through virtual slot 1. Names are hypotheses from this function alone.
//
// MSVCR71 imports (IAT 0x0070c5ac vprintf, 0x0070c580 vsprintf). stdarg.h is
// unavailable, so the argument pointer is a plain char*.

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
