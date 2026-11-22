// sub_00480930: EXE 0x00480930, 18 bytes. Default constructor, __thiscall,
// no parameters, returns this in EAX.
//
// Stores the -1 handle sentinel at +0x20 first (immediate store), then
// zeroes +0x18 and +0x14 from one zeroed register; +0x1c is untouched
// padding. The same record is released by sub_00480950 (frees +0x14/+0x18,
// closes the +0x20 handle) and reopened by sub_004809d0 (CreateFileA on the
// name at +0x24, handle stored at +0x20), so this is the constructor of
// that file-wrapper record. Callers include 0x00463900, 0x0048ab60,
// 0x004d3350, and 0x005c2770.

class Object00480930 {
public:
    unsigned char unknown_00[0x14];
    void* field_14;
    void* field_18;
    unsigned char unknown_1c[4];
    void* field_20;
    char field_24[260];

    Object00480930();
};

Object00480930::Object00480930() {
    field_20 = (void*)-1;
    field_18 = 0;
    field_14 = 0;
}
