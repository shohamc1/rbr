#include "layout.h"
struct Object004087a0 { unsigned char unknown_00[0x28]; unsigned char field_28; unsigned char unknown_29[0x1b]; unsigned int field_44; void sub_004087a0(unsigned int); };
LAYOUT_OFFSET(Object004087a0, field_28, 0x28);
LAYOUT_OFFSET(Object004087a0, field_44, 0x44);

struct Buffer00408d00 {
    unsigned char unknown_00[0x28];
    unsigned int field_28, field_2c;
    unsigned int *data;
    unsigned int flags;
    unsigned int field_38, field_3c;
};
struct Owner00408d00 { Buffer00408d00 *buffer; void sub_00408d00(); };
typedef char buffer_data_offset[(unsigned int)&(((Buffer00408d00*)0)->data) == 0x30 ? 1 : -1];
typedef char buffer_flags_offset[(unsigned int)&(((Buffer00408d00*)0)->flags) == 0x34 ? 1 : -1];
extern "C" void *__cdecl memset(void *, int, unsigned int);
#pragma intrinsic(memset)

// FUNCTION: RBR 0x004087a0
void Object004087a0::sub_004087a0(unsigned int value) { field_44 = value; field_28 = 1; }

// NONMATCHING
// FUNCTION: RBR 0x00408d00
void Owner00408d00::sub_00408d00() {
    buffer->field_28 = 0;
    buffer->field_2c = 0;
    memset(buffer->data, 0, (buffer->flags & 0x1fffffff) << 3);
    buffer->field_3c = 0;
    buffer->field_38 = 0;
}
