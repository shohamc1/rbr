// Layout assertions for MSVC 7.1, which has no static_assert or offsetof
// builtin. A failed check makes a typedef array bound negative. Use
// LAYOUT_OFFSET(type, member, offset); and LAYOUT_SIZE(type, size); next to
// the type definition, one per source line.
#pragma once

#define LAYOUT_CAT_(a, b) a##b
#define LAYOUT_CAT(a, b) LAYOUT_CAT_(a, b)
#define LAYOUT_OFFSET(type, member, offset) \
    typedef char LAYOUT_CAT(layout_offset_, __LINE__)[((unsigned int)&(((type*)0)->member) == (offset)) ? 1 : -1]
#define LAYOUT_SIZE(type, size) \
    typedef char LAYOUT_CAT(layout_size_, __LINE__)[(sizeof(type) == (size)) ? 1 : -1]
