// sub_004c5d80: __thiscall member (ECX = this)
// with one reference argument (RET 4).
//
// push_back-style append for the 0x004c5dxx container: +4 is the element
// array start, +8 the used-end cursor, +0xc the storage end, four-byte
// elements (size and capacity are pointer differences SAR 2, guarded by a
// null start). With spare capacity the value is stored at the end cursor and
// the cursor advances one element; otherwise the growth helper sub_004c51c0
// (thiscall, three stack words: insert position, count 1, value reference)
// reallocates. The null-start fold sends an empty container straight to the
// growth helper.
class Object004c5d80 {
public:
    unsigned char unknown_00[4];
    unsigned int *field_04;
    unsigned int *field_08;
    unsigned int *field_0c;

    void sub_004c51c0(unsigned int *where, unsigned int count,
                      const unsigned int &value);

    void sub_004c5d80(const unsigned int &value);
};

// NONMATCHING
// FUNCTION: RBR 0x004c5d80
void Object004c5d80::sub_004c5d80(const unsigned int &value) {
    unsigned int size;
    unsigned int capacity;
    if (field_04 == 0)
        size = 0;
    else
        size = (unsigned int)(field_08 - field_04);
    if (field_04 == 0)
        capacity = 0;
    else
        capacity = (unsigned int)(field_0c - field_04);
    if (size < capacity) {
        unsigned int *last = field_08;
        *last = value;
        field_08 = last + 1;
    } else {
        sub_004c51c0(field_08, 1, value);
    }
}
