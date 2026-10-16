struct Record004163c0 {
    unsigned char unknown_00[0x10];
    unsigned int field_10;
    unsigned char unknown_14[0x60];
};

struct Collection004163c0 {
    unsigned char unknown_00[0x10];
    Record004163c0* records;
    unsigned char unknown_14[0xcc];
    unsigned int maximum_index;
    void sub_004163c0(unsigned int index, unsigned int value);
};

typedef char record_size[(sizeof(Record004163c0) == 0x74) ? 1 : -1];
typedef char record_field_offset[(sizeof(((Record004163c0*)0)->unknown_00) == 0x10) ? 1 : -1];
#if defined(_M_IX86) || defined(__i386__)
typedef char records_offset[(unsigned int)&(((Collection004163c0*)0)->records) == 0x10 ? 1 : -1];
typedef char maximum_offset[(unsigned int)&(((Collection004163c0*)0)->maximum_index) == 0xe0 ? 1 : -1];
#endif

void Collection004163c0::sub_004163c0(unsigned int index, unsigned int value) {
    records[index].field_10 = value;
    if (index <= maximum_index) return;
    maximum_index = index;
}
