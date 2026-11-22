// sub_00401020: EXE 0x00401020, 11 bytes. MSVC 7.1 (/O2 /Zl).
//
// thiscall predicate on the receiver: loads the dword at +0x4, zero-extends a
// != 0 test into EAX (XOR EAX,EAX; TEST EDX,EDX; SETNZ AL), bare RET. The
// class is the header whose +0x4 slot holds a pointer-or-count tested by
// callers; see sub_004021e0/sub_004022f0 which free the same slot in their
// deleting destructors (vtables 0x0076f3f0/0x0076f3e8).

struct Object00401020 {
    unsigned char field_00[0x4];
    void* field_04;
    bool sub_00401020() const;
};

bool Object00401020::sub_00401020() const {
    return field_04 != 0;
}
