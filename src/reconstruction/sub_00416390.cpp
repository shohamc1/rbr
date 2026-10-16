struct Record00416390 {
    unsigned int field_00;
    unsigned char unknown_04[0x70];
};

struct Collection00416390 {
    unsigned char unknown_00[0x10];
    Record00416390* records;
    unsigned char unknown_14[0xcc];
    unsigned int maximum_index;
    void sub_00416390(unsigned int index, unsigned int value);
};

typedef char record_size[(sizeof(Record00416390) == 0x74) ? 1 : -1];
#if defined(_M_IX86) || defined(__i386__)
typedef char records_offset[(unsigned int)&(((Collection00416390*)0)->records) == 0x10 ? 1 : -1];
typedef char maximum_offset[(unsigned int)&(((Collection00416390*)0)->maximum_index) == 0xe0 ? 1 : -1];
#endif

void Collection00416390::sub_00416390(unsigned int index, unsigned int value) {
    records[index].field_00 = value;
    if (index <= maximum_index) return;
    maximum_index = index;
}
