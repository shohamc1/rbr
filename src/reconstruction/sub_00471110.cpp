// WinMain, called by the CRT entry point with four stack arguments and RET 0x10.
// Reads the run modes from RichardBurnsRally.ini, checks the SSE CPU feature with
// CPUID, initializes COM, creates the 0x1bd0-byte platform object, hands it the
// command line, and runs the game loop.
//
// Known byte-level blockers, probed against the pinned MSVC 13.10.3077 compiler:
// - CPUID(1) at 0x00471360 (MOV EAX,1; CPUID; use EDX): MSVC 7.1 has no __cpuid
//   intrinsic (warning C4163 probed) and inline assembly is forbidden. The
//   placeholder call below stands in; the function cannot match because of it.
// - The std::basic_string constructor and destructor calls go through the
//   MSVCP71 import table. The explicit specialization below reproduces the
//   exact mangled import names and the CALL [__imp_...] form of the reference.
// - The CPUID placeholder has no manifest address. C++ EH data and funclets
//   also need linker-aware checking.

namespace std {
    template<class Element> struct char_traits;
    template<class Element> class allocator;
    template<class Element, class Traits, class Allocator> class basic_string;
    // The EXE imports these two member functions from MSVCP71.dll. char_traits
    // must be a struct (mangles as U) and allocator a class (mangles as V) to
    // keep the mangled names byte-identical to the import slots.
    template<> class __declspec(dllimport) basic_string<char, char_traits<char>, allocator<char> > {
        char buffer[0x10];
        unsigned int length;
        unsigned int capacity;
        unsigned int allocatorField;
    public:
        basic_string(const char* text);
        ~basic_string();
    };
}

typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > IniString;
typedef char ini_string_size_assertion[(sizeof(IniString) == 0x1c) ? 1 : -1];

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
extern "C" void* __cdecl sub_006efa32(unsigned int size); // operator new; manifest symbol resolves
extern "C" unsigned int __cdecl sub_00471110_missingCpuid(int level); // returns the EDX leaf

// Stand-in for the window handle getter used at 0x007c9fe8+0x28; the manifest
// pins the exact mangled symbol for this member.
struct Object0040e570 {
    unsigned int sub_0040e570();
};

struct AppWindow007c9fe8 {
    unsigned char unknown_00[0x28];
    Object0040e570* window;
};
typedef char app_window_field_assertion[
    ((unsigned int)&((AppWindow007c9fe8*)0)->window) == 0x28 ? 1 : -1];

// The ini reader object lives on this function's stack at [esp+0x4]; its
// constructor (sub_0040f9f0) and destructor (sub_0040fa60) are thiscalls.
struct Object0040f9f0 {
    void* field_00;
    void* sub_0040f9f0();
    void sub_0040fa60();
    void sub_0040fb60(const IniString& fileName, int mode);
    bool sub_004100e0(const IniString& section, const IniString& key, int fallback);
};

// The 0x1bd0-byte platform object created below. delete uses vtable slot 0 with
// argument 1 (the deleting destructor); the checked call uses slot 1.
struct GamePlatform0044a3c0 {
    virtual ~GamePlatform0044a3c0();
    virtual unsigned int platformInit();
    virtual void run();
    virtual void shutdown();
    GamePlatform0044a3c0* sub_0044a3c0(void* instance); // constructor, returns this
    void sub_00446bd0(const IniString& arguments);
    unsigned int sub_0047e860();
    unsigned int sub_0047eb00();
    void sub_00479e00();
    void sub_004807c0();
};

// The reference duplicates this block before the "Could not initialize COM!"
// message and again after SetFocus. A macro keeps the two copies identical.
#define RESTORE_CURSOR_AND_WINDOW() \
    if (*(unsigned char*)0x007ca1fc != 0) { \
        *(unsigned char*)0x007ca1fc = 0; \
        if (*(void**)0x007ca1f8 != 0) { \
            ShowCursor(1); \
            DestroyWindow(*(void**)0x007ca1f8); \
            *(void**)0x007ca1f8 = 0; \
        } \
    }

extern "C" int __stdcall sub_00471110(void* instance, void* previousInstance,
                                      char* commandLine, int showCommand) {
    bool runIntro = false;
    if (*(unsigned int*)0x007ca1f0 == 0xb7) { // startup guard: main already ran
        return 0;
    }
    *(unsigned int*)0x007a18f8 = 0;
    // The reference pushes the filter address 0x00470f70 as an immediate
    // (sub_00470f70; declared long __stdcall f(void*) by its observed use).
    SetUnhandledExceptionFilter((void*)0x00470f70);

    Object0040f9f0 iniReader;
    iniReader.sub_0040f9f0();
    iniReader.sub_0040fb60(IniString((const char*)0x00710ab4), 2); // "RichardBurnsRally.ini"

    runIntro = iniReader.sub_004100e0( // section "Settings", key "Fullscreen", default on
                       IniString((const char*)0x0070eba4), IniString((const char*)0x0070ebb0), 1)
            && iniReader.sub_004100e0( // section "Settings", key "RunIntro", default on
                       IniString((const char*)0x0070eba4), IniString((const char*)0x00710aa8), 1);

    if (iniReader.sub_004100e0( // section "Settings", key "RunDiagnose", default off
            IniString((const char*)0x0070eba4), IniString((const char*)0x00710a9c), 0)) {
        sub_0041c8a0(0);
    }
    bool runStartup = iniReader.sub_004100e0( // section "Settings", key "RunStartup", default on
            IniString((const char*)0x0070eba4), IniString((const char*)0x00710a90), 1);
    if (runStartup) {
        if (sub_00470f00()) {
            iniReader.sub_0040fa60();
            return 0;
        }
    }
    sub_00470ca0(runIntro, instance);

    if (iniReader.sub_004100e0( // section "Settings", key "RunIntro" again after the startup run
            IniString((const char*)0x0070eba4), IniString((const char*)0x00710aa8), 1)) {
        sub_00470ea0();
        sub_00470e60();
        sub_00470ca0(runIntro, instance);
    }

    if ((sub_00471110_missingCpuid(1) & 0x02000000) == 0) { // reference: MOV EAX,1; CPUID; EDX bit 25
        MessageBoxA(0, (const char*)0x00710968, (const char*)0x00710a04, 0x10);
        iniReader.sub_0040fa60();
        return 0;
    }

    if (CoInitialize(0) < 0) {
        RESTORE_CURSOR_AND_WINDOW()
        MessageBoxA(0, (const char*)0x00710a68, (const char*)0x00710a84, 0);
        iniReader.sub_0040fa60();
        return 1;
    }

    GamePlatform0044a3c0*& platform = *(GamePlatform0044a3c0**)0x007ca2a0;
    void* platformMemory = sub_006efa32(0x1bd0);
    GamePlatform0044a3c0* created = 0;
    if (platformMemory != 0) {
        created = ((GamePlatform0044a3c0*)platformMemory)->sub_0044a3c0(instance);
    }
    platform = created;

    const char* commandLineEnd = commandLine;
    while (*commandLineEnd != '\0') ++commandLineEnd;
    if (commandLineEnd != commandLine) {
        platform->sub_00446bd0(IniString(commandLine));
    }

    if (platform->sub_0047e860() == 0) {
        sub_0043b3c0(7, 0, (const char*)0x00710a4c); // "Error in PrePlatformInit"
    }
    if (platform->platformInit() == 0) {
        sub_0043b3c0(7, 0, (const char*)0x00710a34); // "Error in PlatformInit"
    }

    AppWindow007c9fe8*& application = *(AppWindow007c9fe8**)0x007c9fe8;
    SetForegroundWindow((void*)application->window->sub_0040e570());
    SetFocus((void*)application->window->sub_0040e570());
    RESTORE_CURSOR_AND_WINDOW()

    if (platform->sub_0047eb00() == 0) {
        sub_0043b3c0(7, 0, (const char*)0x00710a18); // "Error in PostPlatformInit"
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
