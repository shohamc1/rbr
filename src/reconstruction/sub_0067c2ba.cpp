// sub_0067c2ba: EXE 0x0067c2ba, 25 bytes. __thiscall member, no parameters.
//
// Assigns each node's +0x30 word its own table index: walks the node pointer
// table at +0x14 for the unsigned count at +0x8 and stores the loop index
// into +0x30 of each node. Both +0x8 reads stay in memory (the store may
// alias). Always returns zero. Callers include 0x0067e8f1, 0x0067ed04,
// 0x00685cb8, and 0x00686978.
//
// The count bound stays a direct memory compare at both the guard and the
// loop tail with no alignment padding: the file-local size pragma selects
// that loop shape.

#pragma optimize("s", on)

class Object0067c2ba {
public:
    struct Node {
        unsigned char unknown_00[0x30];
        unsigned int field_30;
    };
    unsigned char unknown_00[0x8];
    unsigned int field_08;
    unsigned char unknown_0c[0x8];
    Node **field_14;

    unsigned int sub_0067c2ba();
};

unsigned int Object0067c2ba::sub_0067c2ba() {
    for (unsigned int i = 0; i < field_08; i++)
        field_14[i]->field_30 = i;
    return 0;
}
