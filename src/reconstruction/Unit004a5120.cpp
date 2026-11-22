// sub_004a5120: MSVC 7.1 (/O2 /Zl).
//
// thiscall member, two stack arguments (float at [esp+4], unsigned index at
// [esp+8]), RET 8. Writes the value into the two-float array at +0x24 when
// the index is below 2, then restarts a controller: current (+0x2c) seeded
// from +0x24 by an integer register copy, velocity (+0x30) and the state
// words +0x34/+0x38 zeroed (with +0x34 zeroed a second time after its read
// and +0x38 then set to 300.0f), current advanced by the zero-valued product
// with the velocity, and the velocity set to the distance from the target at
// +0x28 scaled by the float constant 1/300 at 0x00714514.
//
// All five float words are volatile: every reference read and write goes
// back to memory (integer-register seed, the zero product re-loaded and
// re-stored, +0x2c re-read after its store), and the read of +0x34 sits
// between its two zero stores. Plain members let /O2 fold the product away
// and fuse the seed into the final x87 subtraction.
//
// NONMATCHING (residual on the two +0x2c reads as binary-op operands): the
// accumulate compiles to `fld [ecx+0x2c]; fadd st(1); fstp st(0)` where the
// reference has `fadd [ecx+0x2c]` — MSVC 7.1 gives a volatile operand an
// explicit FLD when the other operand is a named local (the split product),
// and only keeps the memory form when the other operand is an arithmetic
// subexpression or a volatile leaf. The velocity subtraction has the right
// memory form (`fsub [ecx+0x2c]`, direct +0x28 leaf) but its
// `fld [ecx+0x28]` is scheduled at the subtraction instead of the
// reference's early preload above `fld [ecx+0x34]`. Local experiments that
// fix one side break the other: an in-expression product restores
// `fadd [ecx+0x2c]` but turns the multiply into
// `fld [ecx+0x30]; fmul st(1)` and hoists the +0x38=300 store above the
// multiply; a non-volatile +0x2c keeps the memory forms but lets the
// optimizer forward the stored sum (`fst; fxch; fsub st(1)`); volatile-cast
// reads pop the store yet re-FLD the value. /Op, /Og-, /O1, /Os, and
// #pragma optimize("s") all degrade the zero stores or add spills.

class Object004a5120 {
public:
    unsigned char field_00[0x24];
    volatile float field_24[2]; // +0x24 start value, +0x28 target value
    volatile float field_2c;    // current
    volatile float field_30;    // velocity
    volatile float field_34;    // state
    volatile float field_38;    // state
    void sub_004a5120(float value, unsigned int index);
};

// sub_004a5980: MSVC 7.1 (/O2 /Zl).
//
// thiscall member, no stack args, bare RET. Clears the two words at +0x70
// and +0x74, then releases the sub-object pointer at +0x88: a no-argument
// virtual call through vtable slot 0x54 of the sub-object, and — when the
// pointer survived that call (it is reloaded and re-tested) — a virtual call
// through slot 0x58 of the manager object behind the global pointer at
// 0x008b4998, passing the sub-object; finally nulls the field.
//
// sub_004a5980 itself sits at slot 0 of the vtable at 0x0071463c (entries
// 0x004a5980, 0x004a5c20, 0x004a5d70, 0x004a5e80, 0x004a5eb0), so the
// receiver is polymorphic with that table.

// Sub-object type: vtable slot 0x54 (22nd) is a no-argument thiscall member.
class SubObject004a5980 {
public:
    virtual void vslot_00(); virtual void vslot_01(); virtual void vslot_02();
    virtual void vslot_03(); virtual void vslot_04(); virtual void vslot_05();
    virtual void vslot_06(); virtual void vslot_07(); virtual void vslot_08();
    virtual void vslot_09(); virtual void vslot_10(); virtual void vslot_11();
    virtual void vslot_12(); virtual void vslot_13(); virtual void vslot_14();
    virtual void vslot_15(); virtual void vslot_16(); virtual void vslot_17();
    virtual void vslot_18(); virtual void vslot_19(); virtual void vslot_20();
    virtual void vslot_54();
};

// Manager type behind *(void**)0x008b4998: vtable slot 0x58 (23rd) takes the
// sub-object as its only stack argument.
class Manager008b4998 {
public:
    virtual void vslot_00(); virtual void vslot_01(); virtual void vslot_02();
    virtual void vslot_03(); virtual void vslot_04(); virtual void vslot_05();
    virtual void vslot_06(); virtual void vslot_07(); virtual void vslot_08();
    virtual void vslot_09(); virtual void vslot_10(); virtual void vslot_11();
    virtual void vslot_12(); virtual void vslot_13(); virtual void vslot_14();
    virtual void vslot_15(); virtual void vslot_16(); virtual void vslot_17();
    virtual void vslot_18(); virtual void vslot_19(); virtual void vslot_20();
    virtual void vslot_21();
    virtual void vslot_58(SubObject004a5980* object);
};

extern "C" Manager008b4998* gManager008b4998; // EXE .data 0x008b4998

class Object004a5980 {
public:
    unsigned char field_00[0x70];
    unsigned int field_70;
    unsigned int field_74;
    unsigned char field_78[0x10];
    SubObject004a5980* field_88;
    void sub_004a5980();
};

// sub_004a5ee0: MSVC 7.1 (/O2 /Zl).
//
// thiscall two-word setter: copies the first stack argument to +0x10 and the
// second to +0x14 in order, RET 8. No incoming references in the Ghidra
// index; the argument kinds (words versus pointers) are not evidenced, so
// they are modelled as unsigned ints.

class Object004a5ee0 {
public:
    unsigned char field_00[0x10];
    unsigned int field_10;
    unsigned int field_14;
    void sub_004a5ee0(unsigned int first, unsigned int second);
};

// sub_004a5f00: MSVC 7.1 (/O2 /Zl).
//
// Default constructor (ECX in, EAX = this out, bare RET) of an intrusive-list
// head: zeroes last (+4), first (+0), then count (+8) — the batch-100
// allocator layout; push_back sub_004a5f10 reads the same fields. The
// MOV EAX,ECX prologue is the constructor form; no incoming references in
// the Ghidra index.

class Object004a5f00 {
public:
    void* field_00;        // first node
    void* field_04;        // last node
    unsigned int field_08; // count
    Object004a5f00();
};

// sub_004a5f10: MSVC 7.1 (/O2 /Zl).
//
// thiscall push_back (RET 4): appends a node at the tail of an intrusive
// doubly-linked list. Container layout (batch-100 allocator family): first
// +0, last +4, count +8; nodes: next +4, prev +8, owner +0xc. The new node
// always gets prev = old last and next = 0; a non-null old last is forwarded
// to the new node, otherwise the node also becomes the container first. The
// node's owner is the container and the count is incremented in both paths.
// No incoming references in the Ghidra index; an embedded instance of this
// container at +0x208 is driven by sub_004a5fb0, which inlines this body.

struct Node004a5f10 {
    unsigned int field_00;
    Node004a5f10* field_04; // next
    Node004a5f10* field_08; // prev
    void* field_0c;         // owning container
};

class Object004a5f10 {
public:
    Node004a5f10* field_00; // first
    Node004a5f10* field_04; // last
    unsigned int field_08;  // count
    void sub_004a5f10(Node004a5f10* node);
};

// sub_004a5fb0: MSVC 7.1 (/O2 /Zl).
//
// thiscall member (RET 4): appends a node at the tail of the intrusive list
// embedded at +0x208 of the receiver. The body is the sub_004a5f10 push_back
// inlined with its container `this' adjusted to receiver+0x208 (the count at
// +0x210 and the node's owner both use the adjusted address); the old last is
// read through the unadjusted receiver (+0x20c) before the adjustment. Node
// layout: next +4, prev +8, owner +0xc. No incoming references in the Ghidra
// index.

struct Node004a5fb0 {
    unsigned int field_00;
    Node004a5fb0* field_04; // next
    Node004a5fb0* field_08; // prev
    void* field_0c;         // owning container
};

class List004a5fb0 {
public:
    Node004a5fb0* field_00; // first
    Node004a5fb0* field_04; // last
    unsigned int field_08;  // count
};

class Object004a5fb0 {
public:
    unsigned char field_00[0x20c];
    Node004a5fb0* field_20c; // last node of the embedded list
    void sub_004a5fb0(Node004a5fb0* node);
};

// sub_004a6060: MSVC 7.1 (/O2 /Zl).
//
// thiscall member, no stack args. Release: when the owning container pointer
// at +0x08 is non-null, decrements the container count at its +0x08 and
// tail-jumps into the gap-closing unlink sub_00454870 (byte-identical second
// copy of sub_00453b50's body, stub in config/symbols.csv) with ECX still holding
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

// NONMATCHING
// FUNCTION: RBR 0x004a5120
void Object004a5120::sub_004a5120(float value, unsigned int index)
{
    if (index < 2)
        field_24[index] = value;
    field_2c = field_24[0];
    field_30 = 0.0f;
    field_34 = 0.0f;
    field_38 = 0.0f;
    float delta = field_34;
    field_34 = 0.0f;
    delta = delta * field_30;
    field_38 = 300.0f;
    field_2c = delta + field_2c;
    field_30 = (field_24[1] - field_2c) * (1.0f / 300.0f);
}

// FUNCTION: RBR 0x004a5980
void Object004a5980::sub_004a5980()
{
    field_70 = 0;
    field_74 = 0;
    if (field_88 != 0) {
        field_88->vslot_54();
        if (field_88 != 0) {
            gManager008b4998->vslot_58(field_88);
            field_88 = 0;
        }
    }
}

// FUNCTION: RBR 0x004a5ee0
void Object004a5ee0::sub_004a5ee0(unsigned int first, unsigned int second)
{
    field_10 = first;
    field_14 = second;
}

// FUNCTION: RBR 0x004a5f00
Object004a5f00::Object004a5f00()
{
    field_04 = 0;
    field_00 = 0;
    field_08 = 0;
}

// FUNCTION: RBR 0x004a5f10
void Object004a5f10::sub_004a5f10(Node004a5f10* node)
{
    Node004a5f10* last = field_04;
    node->field_08 = last;
    node->field_04 = 0;
    if (last != 0) {
        last->field_04 = node;
        field_04 = node;
    } else {
        field_00 = node;
        field_04 = node;
    }
    node->field_0c = this;
    field_08++;
}

// FUNCTION: RBR 0x004a5fb0
void Object004a5fb0::sub_004a5fb0(Node004a5fb0* node)
{
    Node004a5fb0* last = field_20c;
    List004a5fb0* self = (List004a5fb0*)((unsigned char*)this + 0x208);
    node->field_08 = last;
    node->field_04 = 0;
    if (last != 0) {
        last->field_04 = node;
        self->field_04 = node;
    } else {
        self->field_00 = node;
        self->field_04 = node;
    }
    node->field_0c = self;
    self->field_08++;
}

// FUNCTION: RBR 0x004a6060
void Object004a6060::sub_004a6060()
{
    if (field_08 != 0) {
        field_08->field_08--;
        ((Object00454870*)this)->sub_00454870();
    }
}
