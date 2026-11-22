// sub_0061c18e: MSVC 7.1 (/O2 /Zl) with the
// file-local size pragma selecting the POP ECX cdecl cleanups.
//
// Array-holder destroy member of the family whose sibling at 0x006128b1
// passes (this, 0x14, stride, callback 0x0061d11f) the same way.
// __thiscall with one stack word (RET 4). ECX is the holder; the flags
// byte is the stack word. When bit 1 is set, the element-destructor helper
// sub_00610b82 runs with four stack words (base = this, count = 0xc,
// stride = the dword at this-4, callback 0x006195ed, which frees *(ECX));
// bit 0 then frees the block header at this-4, and the header pointer is
// returned. Otherwise *this is freed, bit 0 frees this itself, and this is
// returned.


extern "C" void __stdcall sub_00610b82(void* base, unsigned int count,
                                       unsigned int stride, void* callback);
extern "C" void __cdecl sub_006efdd0(void* memory);

extern "C" void __cdecl sub_006195ed(); // element destructor, a thiscall body (PUSH [ECX]; CALL delete)

class Object0061c18e {
public:
    void* sub_0061c18e(unsigned char flags);
};

// sub_0061cf56: MSVC 7.1 (/O2 /Zl).
//
// Zero-initializer member returning this: clears the five dwords at
// +0x00..+0x10 and returns the receiver in EAX (ECX-in/EAX-out shape).
// Callers (0x00612576, 0x00610227, 0x0061030e, 0x00610bbf, 0x00610dd5)
// pass the object in ECX; no stack arguments (bare RET).

class Object0061cf56 {
public:
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
    unsigned int field_10;

    Object0061cf56* sub_0061cf56();
};

// sub_0061d0fd: MSVC 7.1 (/O2 /Zl) with the
// file-local size pragma (AND [ESI],0 zero store).
//
// Release member: when the interface pointer at +0 is set, calls table
// slot +0x28 on it, reloads the pointer (the call may change it), calls
// table slot +0x08 on it, clears the pointer, and returns 0. The two table
// calls are __stdcall with the object as the single stack word (callee
// cleans; no caller cleanup after either CALL). Reached by the tail jump
// at 0x0061d611 and called at 0x0061d628.


typedef void (__stdcall* TableFn0061d0fd)(void* self);

struct Table0061d0fd {
    void* slot_00;
    void* slot_04;
    TableFn0061d0fd slot_08;
    void* slot_0c;
    void* slot_10;
    void* slot_14;
    void* slot_18;
    void* slot_1c;
    void* slot_20;
    void* slot_24;
    TableFn0061d0fd slot_28;
};

struct Interface0061d0fd {
    Table0061d0fd* table;
};

class Object0061d0fd {
public:
    Interface0061d0fd* field_00;

    int sub_0061d0fd();
};

// sub_0061d989: MSVC 7.1 (/O2 /Zl) with the
// file-local size pragma (vtable store scheduled between the first
// argument push and its call; batched ADD ESP,0xc cleanup).
//
// Base teardown member of the hierarchy whose deleting destructor
// sub_0061ea99 sits in slot 0 of the vtable at EXE 0x00734090. Stores the
// base vtable word at +0, then frees the pointers at +0x38, +0x58 and
// +0x5c through the CRT operator-delete thunk sub_006efdd0. __thiscall, no
// stack arguments. Called from sub_0061ea99, the tail of sub_0061e6bf, and
// 37 deleting-destructor wrappers in the 0x0062xxxx range.


extern "C" const void* const gVtable00734090[]; // EXE .rdata 0x00734090

extern "C" void __cdecl sub_006efdd0(void* memory);

class Object0061d989 {
public:
    void* vtable_00;
    unsigned char unknown_04[0x34];
    void* field_38;
    unsigned char unknown_3c[0x1c];
    void* field_58;
    void* field_5c;

    void sub_0061d989();
};

#pragma optimize("s", on)
// FUNCTION: RBR 0x0061c18e
void* Object0061c18e::sub_0061c18e(unsigned char flags) {
    if (flags & 2) {
        char* block = (char*)this - 4;
        sub_00610b82(this, 0xc, *(unsigned int*)block, (void*)sub_006195ed);
        if (flags & 1)
            sub_006efdd0(block);
        return block;
    }
    sub_006efdd0(*(void**)this);
    if (flags & 1)
        sub_006efdd0((void*)this);
    return (void*)this;
}
#pragma optimize("", on)

// FUNCTION: RBR 0x0061cf56
Object0061cf56* Object0061cf56::sub_0061cf56() {
    field_00 = 0;
    field_04 = 0;
    field_08 = 0;
    field_0c = 0;
    field_10 = 0;
    return this;
}

#pragma optimize("s", on)
// FUNCTION: RBR 0x0061d0fd
int Object0061d0fd::sub_0061d0fd() {
    Interface0061d0fd* interface = field_00;
    if (interface != 0) {
        interface->table->slot_28(interface);
        interface = field_00;
        if (interface != 0) {
            interface->table->slot_08(interface);
            field_00 = 0;
        }
    }
    return 0;
}
#pragma optimize("", on)

#pragma optimize("s", on)
// FUNCTION: RBR 0x0061d989
void Object0061d989::sub_0061d989() {
    vtable_00 = (void*)gVtable00734090;
    sub_006efdd0(field_38);
    sub_006efdd0(field_58);
    sub_006efdd0(field_5c);
}
#pragma optimize("", on)
