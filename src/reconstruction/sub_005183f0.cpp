// sub_005183f0: EXE 0x005183f0, 84 bytes. MSVC 7.1 (/O2 /Zl).
//
// Visitor sweep over a scene-graph child list. Builds a 16-byte visitor
// record on the stack: dispatch table 0x00720c9c at +0 (slot 1 is
// EXE 0x005171b0), scale 1.0f at +4, receiver+0x10 at +8 (receiver read from
// this+0x28), receiver (this+0x20) at +0xc. Then walks the child list at
// receiver+0x450 (next at +0x10): each child is passed to the visitor's
// slot-1 method (CALL [EDX+4] through the record's table pointer), then the
// child's dispatcher sub_0050c9f0 is called with the visitor record.
// thiscall member, 16-byte local (SUB ESP,0x10), bare RET. Called from
// 0x00518c85.
//
// The visitor record is a plain struct with a raw table constant; the
// slot-1 call goes through an abstract-class view so the compiler emits the
// virtual dispatch without an object-local vtable.

extern "C" const void* const gVtable00720c9c[]; // EXE .rdata 0x00720c9c

class Visitor005183f0 {
public:
    unsigned int field_00;   // dispatch table; 0x00720c9c for this visitor
    float field_04;          // scale; 1.0
    void *field_08;          // receiver + 0x10
    void *field_0c;          // receiver
};

class VisitorTarget005183f0 {
public:
    virtual void slot0() = 0;             // table slot 0, EXE 0x00514da0
    virtual void slot1(void *node) = 0;   // table slot 1, EXE 0x005171b0
};

class Node0050c9f0 {
public:
    void sub_0050c9f0(void *visitor);   // external child dispatcher, EXE 0x0050c9f0
};

class Object005183f0 {
public:
    unsigned char unknown_00[0x20];
    void *field_20;          // receiver object
    unsigned char unknown_24[4];
    void *field_28;          // second record, used at +0x10

    void sub_005183f0();
};

void Object005183f0::sub_005183f0() {
    Visitor005183f0 visitor;
    visitor.field_00 = (unsigned int)gVtable00720c9c;
    visitor.field_04 = 1.0f;
    visitor.field_08 = (char *)field_28 + 0x10;
    void *receiver = field_20;
    visitor.field_0c = receiver;
    for (void *node = *(void **)((char *)receiver + 0x450); node != 0;
         node = *(void **)((char *)node + 0x10)) {
        ((VisitorTarget005183f0 *)&visitor)->slot1(node);
        ((Node0050c9f0 *)node)->sub_0050c9f0(&visitor);
    }
}
