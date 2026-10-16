// Launches "RBR Startup.exe" (literal command line at 0x007108dc) and closes
// both child handles. Called by game main sub_00471110.
extern "C" void *__cdecl memset(void *, int, unsigned int);
#pragma intrinsic(memset)

extern "C" {
__declspec(dllimport) int __stdcall CreateProcessA(
    const void *application_name, void *command_line,
    const void *process_attributes, const void *thread_attributes,
    int inherit_handles, unsigned long creation_flags, const void *environment,
    const void *current_directory, void *startup_info,
    void *process_information);
__declspec(dllimport) int __stdcall CloseHandle(void *handle);
}

struct StartupInformationA {
    unsigned long cb;
    const void *lpReserved;
    void *lpDesktop;
    void *lpTitle;
    unsigned long dwX;
    unsigned long dwY;
    unsigned long dwXSize;
    unsigned long dwYSize;
    unsigned long dwXCountChars;
    unsigned long dwYCountChars;
    unsigned long dwFillAttribute;
    unsigned long dwFlags;
    unsigned short wShowWindow;
    unsigned short cbReserved2;
    unsigned char *lpReserved2;
    void *hStdInput;
    void *hStdOutput;
    void *hStdError;
};

struct ProcessInformation {
    void *hProcess;
    void *hThread;
    unsigned long dwProcessId;
    unsigned long dwThreadId;
};

extern "C" bool __cdecl sub_00470f00() {
    StartupInformationA startup_info;
    ProcessInformation process_info;
    memset(&startup_info, 0, sizeof(startup_info));
    startup_info.cb = sizeof(startup_info);
    memset(&process_info, 0, sizeof(process_info));
    if (!CreateProcessA(0, (void *)0x7108dc, 0, 0, 0, 0, 0, 0, &startup_info,
                        &process_info)) {
        return false;
    }
    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);
    return true;
}
