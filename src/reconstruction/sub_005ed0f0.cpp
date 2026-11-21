// sub_005ed0f0: EXE 0x005ed0f0, 36 bytes. Constructor, two pointer
// parameters (RET 8), ECX-in/EAX-out.
//
// Callback/delegate record: dispatch table at +0 (raw store of the EXE table
// 0x007309c8, whose first slot is the release method at 0x005ed130), an
// embedded three-pointer link node at +4/+8/+0xc (zeroed here; unlinked by
// sub_0043d920 through the wrapper sub_005ed040), the callback receiver at
// +0x10 (passed as ECX), and the callback function at +0x14 (invoked by
// sub_005ed120).

class Object005ed0f0 {
public:
    void *field_00;
    void *field_04;
    void *field_08;
    void *field_0c;
    void *field_10;
    void *field_14;

    Object005ed0f0(void *callback_object, void *callback_function);
};

Object005ed0f0::Object005ed0f0(void *callback_object, void *callback_function) {
    field_08 = 0;
    field_04 = 0;
    field_0c = 0;
    field_00 = (void *)0x007309c8;
    field_10 = callback_object;
    field_14 = callback_function;
}
