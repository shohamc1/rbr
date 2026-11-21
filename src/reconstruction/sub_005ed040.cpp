// sub_005ed040: EXE 0x005ed040, 19 bytes. __thiscall member, no parameters.
//
// Callback-record unlink wrapper (record built by sub_005ed0f0: dispatch
// table at +0, link node at +4). When the link at +0xc still points at its
// owner list, decrements that list's counter at +8 and tail-jumps to the
// node-unlink helper sub_0043d920 (EXE 0x0043d920, 93 bytes) with the
// embedded link node at +4 as `this` (ADD ECX,4).

struct List005ed040 {
    void *field_00;
    void *field_04;
    unsigned int field_08;
};

class Link005ed040 {
public:
    void *field_00;
    void *field_04;
    List005ed040 *field_08;

    void sub_0043d920();
};

class Object005ed040 {
public:
    void *field_00;
    Link005ed040 field_04;

    void sub_005ed040();
};

void Object005ed040::sub_005ed040() {
    Link005ed040 *link = &field_04;
    if (link->field_08) {
        --link->field_08->field_08;
        link->sub_0043d920();
    }
}
