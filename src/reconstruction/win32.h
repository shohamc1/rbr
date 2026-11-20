// Win32 declarations shared by the MSVC 7.1 sources; the image has no SDK
// headers. Imports are __declspec(dllimport) so the object calls through the
// import address table, as the EXE does (CALL [__imp__Name@N]). The structures
// follow the documented Win32 layouts (STARTUPINFOA is 0x44 bytes,
// PROCESS_INFORMATION is 0x10).
#pragma once

extern "C" {
__declspec(dllimport) int __stdcall CreateProcessA(
    const void *application_name, void *command_line,
    const void *process_attributes, const void *thread_attributes,
    int inherit_handles, unsigned long creation_flags, const void *environment,
    const void *current_directory, void *startup_info,
    void *process_information);
__declspec(dllimport) int __stdcall CloseHandle(void *handle);
__declspec(dllimport) unsigned long __stdcall WaitForSingleObject(
    void *handle, unsigned long milliseconds);
}

struct StartupInfoA {
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
typedef char startup_info_a_size[(sizeof(StartupInfoA) == 0x44) ? 1 : -1];

struct ProcessInformation {
    void *hProcess;
    void *hThread;
    unsigned long dwProcessId;
    unsigned long dwThreadId;
};
typedef char process_information_size[(sizeof(ProcessInformation) == 0x10) ? 1 : -1];
