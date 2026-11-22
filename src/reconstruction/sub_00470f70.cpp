// Build flags: /GS

// Unhandled-exception filter installed by game main (sub_00471110) as the
// literal 0x00470f70 for SetUnhandledExceptionFilter. Writes CRASHDUMP.TXT
// ("exceptionhandler called!!"), builds "<computer>_<date>.dmp" from the
// computer name and the local time, rewrites CRASHDUMP.TXT with the converted
// filename, and reports (4, filename, current thread id, exception info) to
// the shared dump collector sub_0041af00. Always returns 1
// (EXCEPTION_EXECUTE_HANDLER).
//
// The 0x1e1c-byte frame exceeds one page, so the compiler emits the __chkstk
// probe resolved by the linker symbol at 0x006f00e0. Format strings and
// file names are literal .rdata addresses pushed as immediates; they stay
// literal casts to avoid extern data symbols.
//
// The build flags enable /GS for the cookie at EXE 0x007c9410 and the check
// at 0x006efe25. It also pins the helper addresses used by REL32 calls.

// This MSVC 7.1 package has no native wchar_t without CRT headers.
typedef unsigned short wchar_t;

struct FILE;
struct Tm00470f70 {
    int tm_sec, tm_min, tm_hour, tm_mday, tm_mon, tm_year;
};

extern "C" {
__declspec(dllimport) FILE* __cdecl fopen(const char* fileName, const char* mode);
__declspec(dllimport) int __cdecl fprintf(FILE* file, const char* format, ...);
__declspec(dllimport) int __cdecl fclose(FILE* file);
__declspec(dllimport) void __cdecl _tzset(void);
__declspec(dllimport) void __cdecl _time64(long long* timer);
__declspec(dllimport) Tm00470f70* __cdecl _localtime64(const long long* timer);
__declspec(dllimport) int __cdecl sprintf(char* buffer, const char* format, ...);
__declspec(dllimport) int __cdecl swprintf(wchar_t* buffer, const wchar_t* format, ...);
__declspec(dllimport) int __stdcall GetComputerNameA(char* name, unsigned long* size);
__declspec(dllimport) unsigned long __stdcall GetCurrentThreadId(void);
}

// EXE helpers. sub_0059ee80 copies a byte string
// to a wide string and returns the destination; sub_0059e9d0 copies a wide
// string back to bytes (0xff per out-of-range unit).
extern "C" void __cdecl sub_0059ee80(const char* source, wchar_t* destination);
extern "C" void __cdecl sub_0059e9d0(const wchar_t* source, char* destination);
// __stdcall, cleans 0x10 bytes (RET 0x10 in the reference).
extern "C" void __stdcall sub_0041af00(int kind, const wchar_t* fileName,
                                       unsigned long threadId, void* exceptionInfo);

// FUNCTION: RBR 0x00470f70
extern "C" int __stdcall sub_00470f70(void* exceptionInfo) {
    wchar_t computerNameWide[1024];  // widest buffer, ends at the /GS cookie slot
    char computerName[1024];         // GetComputerNameA buffer
    wchar_t fileNameWide[1024];      // swprintf target "<computer>_<date>.dmp"
    wchar_t emptyWide[260];          // converted copy of the empty byte string
    char fileName[1024];             // converted byte filename for fprintf
    wchar_t dateWide[256];           // converted copy of dateText
    char emptyName[260];             // empty byte string, first byte zeroed
    char dateText[256];              // sprintf target "%d %02d %02d-%02d %02d %02d"
    unsigned int crashTime[2];       // __time64_t storage
    unsigned long computerNameSize;  // in/out for GetComputerNameA

    emptyName[0] = 0;
    FILE* file = fopen("CRASHDUMP.TXT", "wt");
    if (file == 0) {
        return 1;
    }
    fprintf(file, "exceptionhandler called!!");
    fclose(file);
    _tzset();
    _time64((long long*)crashTime);
    Tm00470f70* parts = _localtime64((const long long*)crashTime);
    sprintf(dateText, "%d %02d %02d-%02d %02d %02d",
            parts->tm_year + 1900, parts->tm_mon + 1, parts->tm_mday,
            parts->tm_hour, parts->tm_min, parts->tm_sec);
    computerNameSize = 0x100;
    GetComputerNameA(computerName, &computerNameSize);
    sub_0059ee80(computerName, computerNameWide);
    sub_0059ee80(dateText, dateWide);
    sub_0059ee80(emptyName, emptyWide);
    // STRING: RBR 0x0071090c
    swprintf(fileNameWide, L"%s_%s.dmp", computerNameWide, dateWide);
    file = fopen("CRASHDUMP.TXT", "wt");
    if (file != 0) {
        sub_0059e9d0(fileNameWide, fileName);
        fprintf(file, "exceptionhandler called( %s )!!", fileName);
        fclose(file);
    }
    sub_0041af00(4, fileNameWide, GetCurrentThreadId(), exceptionInfo);
    return 1;
}
