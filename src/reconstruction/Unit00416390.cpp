#include "layout.h"
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
LAYOUT_OFFSET(Collection00416390, records, 0x10);
LAYOUT_OFFSET(Collection00416390, maximum_index, 0xe0);

#include "layout.h"
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
LAYOUT_OFFSET(Collection004163c0, records, 0x10);
LAYOUT_OFFSET(Collection004163c0, maximum_index, 0xe0);

// FUNCTION: RBR 0x00416390
void Collection00416390::sub_00416390(unsigned int index, unsigned int value) {
    records[index].field_00 = value;
    if (index <= maximum_index) return;
    maximum_index = index;
}

// FUNCTION: RBR 0x004163c0
void Collection004163c0::sub_004163c0(unsigned int index, unsigned int value) {
    records[index].field_10 = value;
    if (index <= maximum_index) return;
    maximum_index = index;
}
