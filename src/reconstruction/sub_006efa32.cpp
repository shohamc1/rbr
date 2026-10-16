#pragma optimize("s", on)

extern "C" int __cdecl _callnewh(unsigned int size);
extern "C" void* __cdecl malloc(unsigned int size);

namespace std {
    __declspec(dllimport) void __cdecl _Nomemory();
}

extern "C" void* __cdecl sub_006efa32(unsigned int size) {
    void* block;
    while ((block = malloc(size)) == 0) {
        if (_callnewh(size) == 0) {
            std::_Nomemory();
        }
    }
    return block;
}
