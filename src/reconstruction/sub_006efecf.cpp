#pragma optimize("s", on)

extern "C" unsigned int __cdecl sub_006efea9(unsigned int handler);

extern "C" int __cdecl _atexit(unsigned int handler) {
    return sub_006efea9(handler) == 0 ? -1 : 0;
}
