// sub_004a5980: EXE 0x004a5980, 66 bytes. MSVC 7.1 (/O2 /Zl).
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

class Object004a5980 {
public:
    unsigned char field_00[0x70];
    unsigned int field_70;
    unsigned int field_74;
    unsigned char field_78[0x10];
    SubObject004a5980* field_88;
    void sub_004a5980();
};

void Object004a5980::sub_004a5980()
{
    field_70 = 0;
    field_74 = 0;
    if (field_88 != 0) {
        field_88->vslot_54();
        if (field_88 != 0) {
            (*(Manager008b4998**)0x008b4998)->vslot_58(field_88);
            field_88 = 0;
        }
    }
}
