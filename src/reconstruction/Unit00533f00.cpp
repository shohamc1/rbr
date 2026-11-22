#include "Object00533f00.h"

#include "Object00533f00.h"

// FUNCTION: RBR 0x00533f00
void Object00533f00::sub_00533f00(unsigned int value) { field_c = value; }

// FUNCTION: RBR 0x005341e0
void Object00533f00::sub_005341e0(unsigned int value) { field_8 = value; }

// FUNCTION: RBR 0x005342a0
unsigned int Object00533f00::sub_005342a0() { return field_8 == 0; }

// FUNCTION: RBR 0x00534390
unsigned int Object00533f00::sub_00534390() { return field_8 == 5; }
