// Error-reporting wrapper: forwards a formatted-message call to the logging
// core at sub_0048cc00 (cdecl, four stack arguments, the last being the
// variadic argument pointer). Callers pass (7, 0, message) with no variadic
// arguments, e.g. "Error in PlatformInit".
//
// The variadic argument pointer is the MSVC x86 va_start expansion
// (ap = (va_list)&lastParam + 4). stdarg.h is unavailable: the pinned MSVC 7.1
// Docker image mounts no include path.

extern "C" void __cdecl sub_0048cc00(int kind, int code, const char *message,
                                     char *arguments);

extern "C" void __cdecl sub_0043b3c0(int kind, int code, const char *message,
                                     ...) {
    char *arguments = reinterpret_cast<char *>(&message) + 4;
    sub_0048cc00(kind, code, message, arguments);
}
