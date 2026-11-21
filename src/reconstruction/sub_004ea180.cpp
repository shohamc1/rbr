// sub_004ea180: EXE 0x004ea180, 9 bytes. Default constructor, __thiscall,
// no parameters, ECX-in/EAX-out (MOV EAX,ECX prologue).
// Seeds field_00 with the vtable candidate at EXE 0x0071be94 (slot 0 =
// sub_004ea1b0, the deleting destructor; COL at 0x0071be90), following the
// raw-vtable idiom of the matched sub_004995c0 (vtable 0x007137b4).
class Object004ea180 {
public:
    Object004ea180();

private:
    unsigned int vtable;
};

Object004ea180::Object004ea180()
{
    vtable = 0x71be94;
}
