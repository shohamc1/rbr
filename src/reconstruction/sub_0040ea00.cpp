struct Inner0040ea00 {
    unsigned char unknown_00[0x30];
    unsigned int field_30;
    unsigned int field_34;
    unsigned int field_38;
    unsigned int field_3c;
    unsigned int field_40;
    unsigned int field_44;
    unsigned int field_48;
    unsigned int field_4c;
    unsigned char unknown_50[0x10];
    unsigned int field_60;
    unsigned int field_64;
    unsigned int field_68;
    unsigned int field_6c;
    unsigned int field_70;
    unsigned int field_74;
};

struct Object0040ea00 {
    unsigned char unknown_00[0x6e4];
    Inner0040ea00* inner;
    void sub_0040ea00();
};

typedef char object_inner_offset[(unsigned int)&(((Object0040ea00*)0)->inner) == 0x6e4 ? 1 : -1];
typedef char inner_field_74[(unsigned int)&(((Inner0040ea00*)0)->field_74) == 0x74 ? 1 : -1];

void Object0040ea00::sub_0040ea00() {
    Inner0040ea00* value = inner;
    value->field_38 = 0;
    value->field_34 = 0;
    value->field_30 = 0;
    value->field_3c = 0;
    value->field_40 = 0;
    value->field_44 = 0;
    value->field_48 = 0;
    value->field_4c = 0;
    value->field_64 = 0;
    value->field_68 = 0;
    value->field_6c = 0;
    value->field_70 = 0;
    value->field_74 = 0;
}
