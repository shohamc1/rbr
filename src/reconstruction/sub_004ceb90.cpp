// sub_004ceb90: EXE 0x004ceb90, 55 bytes. thiscall member with two dword
// stack arguments (RET 8), returns a dword.
//
// Selector-indexed read from three parallel dword arrays in the receiver:
// selector 0 reads the table at +0xb00, selector 1 the table at +0xb08, and
// anything else the table at +0xb10, in each case at [this + table +
// index*4]. TEST/JZ then CMP/JNZ chain; each path reloads the index
// (EAX, then EDX, then EAX).
class Object004ceb90 {
public:
    unsigned int sub_004ceb90(unsigned int selector, unsigned int index);
};

unsigned int Object004ceb90::sub_004ceb90(unsigned int selector,
                                         unsigned int index) {
    if (selector == 0) {
        return *(unsigned int *)((char *)this + 0xb00 + index * 4);
    }
    if (selector == 1) {
        return *(unsigned int *)((char *)this + 0xb08 + index * 4);
    }
    return *(unsigned int *)((char *)this + 0xb10 + index * 4);
}
