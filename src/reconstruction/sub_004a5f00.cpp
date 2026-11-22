// sub_004a5f00: EXE 0x004a5f00, 13 bytes. MSVC 7.1 (/O2 /Zl).
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

Object004a5f00::Object004a5f00()
{
    field_04 = 0;
    field_00 = 0;
    field_08 = 0;
}
