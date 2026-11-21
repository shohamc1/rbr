// sub_004a6060: EXE 0x004a6060, 16 bytes. MSVC 7.1 (/O2 /Zl).
//
// thiscall member, no stack args. Release: when the owning container pointer
// at +0x08 is non-null, decrements the container count at its +0x08 and
// tail-jumps into the gap-closing unlink sub_00454870 (byte-identical second
// copy of sub_00453b50's body, manifest source:null) with ECX still holding
// this receiver; a null owner returns immediately. The receiver layout is the
// intrusive-node triple {next +0, prev +4, owner +8} that the unlink body
// reads. sub_004a6070 at 0x004a6070 is a byte-identical twin of this body.

// Container: first +0, last +4, count +8.
class Container004a6060 {
public:
    void* field_00;
    void* field_04;
    unsigned int field_08;
};

// Unlink-self member at EXE 0x00454870 on the same node triple.
class Object00454870 {
public:
    void* field_00;              // next node
    void* field_04;              // prev node
    Container004a6060* field_08; // owning container
    void sub_00454870();
};

class Object004a6060 {
public:
    void* field_00;              // next node
    void* field_04;              // prev node
    Container004a6060* field_08; // owning container
    void sub_004a6060();
};

void Object004a6060::sub_004a6060()
{
    if (field_08 != 0) {
        field_08->field_08--;
        ((Object00454870*)this)->sub_00454870();
    }
}
