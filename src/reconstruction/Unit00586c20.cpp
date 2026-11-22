#include "layout.h"
// Object00586c20: the element type of the array that sub_00587070 and
// sub_00587080 (Object00587070 at +0x4, stride 0x46c) address.
//
// Merge evidence: sub_00587070 returns the address of that array. In function
// 0x00557d10 EAX from its return value becomes `this` for sub_00586c70
// (0x00558290, 0x005582fe, 0x0055839a) and for sub_00586c20 (0x00558316,
// 0x005583bc). Both methods index the word array at +0x424. Names are
// hypotheses.
struct Object00586c20 {
    unsigned char unknown_00[0x424];
    unsigned int storage_base_424;

    void sub_00586c20(unsigned int index, unsigned int value);
    unsigned int sub_00586c70(unsigned int index);
};
LAYOUT_OFFSET(Object00586c20, storage_base_424, 0x424);

#include "Object00587070.h"

#include "Object00587070.h"

// Address of array element `index`. The 32-bit multiply wraps; see
// analysis/notes.md for the native-port caveat.

// FUNCTION: RBR 0x00586c20
void Object00586c20::sub_00586c20(unsigned int index, unsigned int value) {
    unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0x424);
    words[index] = value;
}

// FUNCTION: RBR 0x00586c40
unsigned int sub_00586c40() { return 0; }

// FUNCTION: RBR 0x00586c70
unsigned int Object00586c20::sub_00586c70(unsigned int index) {
    unsigned int *words = reinterpret_cast<unsigned int *>(reinterpret_cast<unsigned char *>(this) + 0x424);
    return words[index];
}

// FUNCTION: RBR 0x00587070
unsigned int* Object00587070::sub_00587070() { return &field_04; }

// FUNCTION: RBR 0x00587080
unsigned int Object00587070::sub_00587080(unsigned int index) { return index * 0x46c + (unsigned int)&field_04; }

// FUNCTION: RBR 0x005877e0
unsigned int Object00587070::sub_005877e0() { return field_11bc; }

// FUNCTION: RBR 0x00587830
unsigned int __stdcall sub_00587830(unsigned int value) { return value <= 8; }
