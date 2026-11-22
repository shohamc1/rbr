#include "layout.h"
struct Object00489e30 { unsigned char unknown_00[4]; unsigned int field_04; unsigned int sub_00489e30(); };
LAYOUT_OFFSET(Object00489e30, field_04, 4);

// FUNCTION: RBR 0x00489e30
unsigned int Object00489e30::sub_00489e30() { return field_04; }

// FUNCTION: RBR 0x00489e50
extern "C" unsigned int sub_00489e50() { return 1; }

// FUNCTION: RBR 0x00489e60
extern "C" unsigned int sub_00489e60() { return 1; }
