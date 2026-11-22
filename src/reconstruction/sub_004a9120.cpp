// sub_004a9120: EXE 0x004a9120, 16 bytes. MSVC 7.1 (/O2 /Zl).
//
// thiscall member, no stack args. Release: when the owning container pointer
// at +0x08 is non-null, decrements the container count at its +0x08 and
// tail-jumps into the gap-closing unlink sub_00453b50 with ECX still holding
// this receiver; a null owner returns immediately. Same shape and receiver
// layout (intrusive-node triple {next +0, prev +4, owner +8}) as the
// sub_004a6060/sub_004a6070 twins, which jump to the byte-identical copy at
// 0x00454870 instead.

// Container: first +0, last +4, count +8.
class Container004a9120 {
public:
    void* field_00;
    void* field_04;
    unsigned int field_08;
};

// Gap-closing unlink member at EXE 0x00453b50 (matched source
// sub_00453b50.cpp) on the same node triple.
class Object00453b50 {
public:
    void* field_00;              // next node
    void* field_04;              // prev node
    Container004a9120* field_08; // owning container
    void sub_00453b50();
};

class Object004a9120 {
public:
    void* field_00;              // next node
    void* field_04;              // prev node
    Container004a9120* field_08; // owning container
    void sub_004a9120();
};

void Object004a9120::sub_004a9120()
{
    if (field_08 != 0) {
        field_08->field_08--;
        ((Object00453b50*)this)->sub_00453b50();
    }
}
