#include "Object004181e0.h"

struct Reader004183d0 {
    const unsigned int* data;
    unsigned int offset;
    void sub_004183d0(unsigned int* output);
};

#include "Object004181e0.h"

// FUNCTION: RBR 0x004181e0
Object004181e0::Object004181e0() { field_00 = 0; field_04 = 0; field_0c = 0; field_10 = 0; field_14 = 0; }

// NONMATCHING
// FUNCTION: RBR 0x004183d0
void Reader004183d0::sub_004183d0(unsigned int* output) {
    const unsigned int* source = (const unsigned int*)((const unsigned char*)data + offset);
    const unsigned int first = source[0];
    const unsigned int second = source[1];
    const unsigned int third = source[2];
    offset += 12;
    output[0] = first;
    output[1] = second;
    output[2] = third;
}

// FUNCTION: RBR 0x00418590
void Object004181e0::sub_00418590(unsigned int value) { field_04 = value; }

// FUNCTION: RBR 0x004185a0
unsigned int Object004181e0::sub_004185a0() { return field_04; }
