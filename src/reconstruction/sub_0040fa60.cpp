extern "C" void __stdcall sub_0040fab0(void* block);
extern "C" void __cdecl sub_006efdd0(void* block);

struct Object0040f9f0 {
    void* field_00;
    void* sub_0040f9f0();
    void sub_0040fa60();
};

void Object0040f9f0::sub_0040fa60() {
    void* block = field_00;
    if (block != 0) {
        sub_0040fab0(block);
        sub_006efdd0(block);
    }
    field_00 = 0;
}
