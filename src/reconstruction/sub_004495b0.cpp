extern "C" const void* const gVtable0070e994[]; // EXE .rdata 0x0070e994

struct Object004495b0 {
    unsigned int vtable, field_04, field_08, field_0c, field_10, field_14;
    Object004495b0(unsigned int first, unsigned int second);
};
Object004495b0::Object004495b0(unsigned int first, unsigned int second) {
    field_08 = 0;
    field_04 = 0;
    field_0c = 0;
    vtable = (unsigned int)gVtable0070e994;
    field_10 = first;
    field_14 = second;
}
