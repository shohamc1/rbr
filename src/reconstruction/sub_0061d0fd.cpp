// sub_0061d0fd: EXE 0x0061d0fd, 34 bytes. MSVC 7.1 (/O2 /Zl) with the
// file-local size pragma (AND [ESI],0 zero store).
//
// Release member: when the interface pointer at +0 is set, calls table
// slot +0x28 on it, reloads the pointer (the call may change it), calls
// table slot +0x08 on it, clears the pointer, and returns 0. The two table
// calls are __stdcall with the object as the single stack word (callee
// cleans; no caller cleanup after either CALL). Reached by the tail jump
// at 0x0061d611 and called at 0x0061d628.

#pragma optimize("s", on)

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
