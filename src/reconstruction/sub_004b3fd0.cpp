// Object004b3fd0 member: EXE 0x004b3fd0, 126 bytes. __thiscall with one
// dword stack argument (RET 4), void return; ESI/EDI callee-saved frame.
// Referenced only from the dispatch table slot at 0x00715944 (sibling
// entries 0x004b3770-0x004b44e0), so it is a virtual method body.
//
// Reads this+0xd0 as a pointer to a dword id table, indexes it with the
// argument, and asks the global device object at *(void **)0x008b4998
// (virtual slot 0x50; the graphics/render core used all over the EXE) to
// create an object from that id with fixed arguments (8, 0). It then
// configures the returned object through its vtable slots
// 0x8c, 0x80 (buffer at this+0x80), 0x60 (global float 0x01640460),
// 0x64, 0x74 (17.0f), 0x78 (5.0f), and finally 0x50. The +0xd0 pointer
// load followed by a separate LEA/index load shows the table is external,
// not an inline array. Slot names are hypotheses; the offsets are exact.
//
// Encoding lever: binding &field_d0[index] in a pointer local (entry) is
// load-bearing. The direct expression field_d0[index] fuses the element
// load into one scaled MOV, hoists the index into ESI at the prologue, and
// moves the 0x008b4998 load ahead of it; the local pins the reference
// order: table load, late [ESP+0xc] index load, LEA, global load, element
// load. The same shape appears in sibling slots 0x004b3f40/0x004b4050/
// 0x004b40e0/0x004b4170.

struct Device004b3fd0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b();
    virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void *slot_50(unsigned int id, int a, int b);
};

struct Sink004b3fd0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b();
    virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void slot_50(int a);
    virtual void v15b(); virtual void v16b(); virtual void v17b();
    virtual void slot_60(float a);
    virtual void slot_64(int a);
    virtual void v26(); virtual void v27(); virtual void v28();
    virtual void slot_74(float a);
    virtual void slot_78(float a);
    virtual void v31();
    virtual void slot_80(void *a);
    virtual void v33(); virtual void v34();
    virtual void slot_8c(int a);
};

extern "C" Device004b3fd0* gManager008b4998; // EXE .data 0x008b4998, see sub_004a5980
extern "C" float gFloat01640460;               // EXE .data 0x01640460

struct Object004b3fd0 {
    unsigned char field_00[0x80];
    unsigned char field_80[0x50];
    unsigned int *field_d0;
    void sub_004b3fd0(unsigned int index);
};

// FUNCTION: RBR 0x004b3fd0
void Object004b3fd0::sub_004b3fd0(unsigned int index) {
    unsigned int *entry = field_d0 + index;
    Sink004b3fd0 *object = (Sink004b3fd0 *)
        gManager008b4998->slot_50(*entry, 8, 0);
    object->slot_8c(1);
    object->slot_80(field_80);
    object->slot_60(gFloat01640460);
    object->slot_64(0);
    object->slot_74(17.0f);
    object->slot_78(5.0f);
    object->slot_50(0);
}
