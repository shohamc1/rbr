#pragma optimize("s", on)

extern "C" unsigned int __cdecl sub_006f06ac(unsigned int handler, void* begin, void* end);

extern "C" __declspec(dllimport) unsigned int __cdecl _onexit(unsigned int handler);

extern "C" unsigned int __cdecl sub_006efea9(unsigned int handler) {
    if (*(void**)0x0016e3510u == (void*)-1) {
        return _onexit(handler);
    }
    return sub_006f06ac(handler, (void*)0x0016e3510u, (void*)0x0016e350cu);
}
