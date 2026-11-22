// sub_00569f70: EXE 0x00569f70, 108 bytes. __cdecl callback (caller
// 0x004c53ca) in the quickrace event neighborhood. Reads the game platform
// instance at 0x007ca2a0 (GamePlatform0044a3c0) and switches on its dword at
// +0x728:
//   == 2 - virtual slot 9 (vtable offset 0x24) of the container at
//          *(0x0163f2a0) with argument 0x37 returns an element;
//   else  - non-virtual sub_0054ece0 on the container at *(0x0163f0a8) with
//          argument 0x73 returns the element.
// Each branch then calls virtual slot 0x58 on the object referenced by the
// element's +0xc holder (holder->first->slot_58) — the sequences stay
// duplicated, the branches join only at the result move — and reloads its
// container pointer. If the container's field at +0xc equals the element,
// sub_0056f440 runs on it (ECX carried from the reload; its manifest class
// is the shared Object0056f440). A nonzero slot-0x58 result is tail-jumped
// into sub_00569d50 with ECX = result.
// Compiled with MSVC 7.1 (/O2 /Zl).

class Target00569f70 {
public:
    void sub_00569d50();   // EXE 0x00569d50
};

class Dispatch00569f70 {
public:
    virtual void *slot_00(); virtual void *slot_01(); virtual void *slot_02();
    virtual void *slot_03(); virtual void *slot_04(); virtual void *slot_05();
    virtual void *slot_06(); virtual void *slot_07(); virtual void *slot_08();
    virtual void *slot_09(); virtual void *slot_0a(); virtual void *slot_0b();
    virtual void *slot_0c(); virtual void *slot_0d(); virtual void *slot_0e();
    virtual void *slot_0f(); virtual void *slot_10(); virtual void *slot_11();
    virtual void *slot_12(); virtual void *slot_13(); virtual void *slot_14();
    virtual void *slot_15();
    virtual Target00569f70 *slot_58();  // vtable slot 0x58/4 = 22
};

class Holder00569f70 {
public:
    Dispatch00569f70 *first;   // holder dword 0: referenced object
};

class Element00569f70 {
public:
    void *field_00;
    void *field_04;
    void *field_08;
    Holder00569f70 *field_0c;   // +0xc: holder of the dispatch object
};

class Container00569f70 {
public:
    unsigned int field_04;
    unsigned int field_08;
    Element00569f70 *field_0c;   // +0xc: current element

    virtual void *slot_00(); virtual void *slot_01(); virtual void *slot_02();
    virtual void *slot_03(); virtual void *slot_04(); virtual void *slot_05();
    virtual void *slot_06(); virtual void *slot_07(); virtual void *slot_08();
    virtual Element00569f70 *slot_24(unsigned int index);  // vtable slot 9

    void *sub_0054ece0(unsigned int index);   // EXE 0x0054ece0
};

extern "C" char* gPlatform;                     // EXE .data 0x007ca2a0
extern "C" Container00569f70* gContainer0163f2a0; // EXE .data 0x0163f2a0
extern "C" Container00569f70* gObject0163f0a8;    // EXE .data 0x0163f0a8

class Object0056f440 {
public:
    void sub_0056f440();   // EXE 0x0056f440, shared manifest class
};

extern "C" void __cdecl sub_00569f70() {
    Container00569f70 *container;
    Element00569f70 *element;
    Target00569f70 *result;
    if (*(unsigned int *)(gPlatform + 0x728) == 2) {
        container = gContainer0163f2a0;
        element = container->slot_24(0x37);
        result = element->field_0c->first->slot_58();
        container = gContainer0163f2a0;
    } else {
        container = gObject0163f0a8;
        element = (Element00569f70 *)container->sub_0054ece0(0x73);
        result = element->field_0c->first->slot_58();
        container = gObject0163f0a8;
    }
    if (container->field_0c == element) {
        ((Object0056f440 *)container)->sub_0056f440();
    }
    if (result != 0) {
        result->sub_00569d50();
    }
}
