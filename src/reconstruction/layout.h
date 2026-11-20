// Layout assertions for headers that both the Clang and MSVC 7.1 profiles
// include. Clang has static_assert and __builtin_offsetof. MSVC 7.1 has
// neither, so a failed check there makes a typedef array bound negative.
// Use LAYOUT_OFFSET(type, member, offset); and LAYOUT_SIZE(type, size); next
// to the type definition. Each use must sit on its own source line.
#pragma once

#if defined(__clang__)
// Classes with virtual members are not standard-layout; the offsets still hold.
#define LAYOUT_OFFSET(type, member, offset) \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Winvalid-offsetof\"") \
    static_assert(__builtin_offsetof(type, member) == (offset), #type "::" #member " offset") \
    _Pragma("clang diagnostic pop")
#define LAYOUT_SIZE(type, size) static_assert(sizeof(type) == (size), #type " size")
#else
#define LAYOUT_CAT_(a, b) a##b
#define LAYOUT_CAT(a, b) LAYOUT_CAT_(a, b)
#define LAYOUT_OFFSET(type, member, offset) \
    typedef char LAYOUT_CAT(layout_offset_, __LINE__)[((unsigned int)&(((type*)0)->member) == (offset)) ? 1 : -1]
#define LAYOUT_SIZE(type, size) \
    typedef char LAYOUT_CAT(layout_size_, __LINE__)[(sizeof(type) == (size)) ? 1 : -1]
#endif
