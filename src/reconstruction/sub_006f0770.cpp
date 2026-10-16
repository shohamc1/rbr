#pragma optimize("s", on)

extern "C" unsigned int __cdecl _controlfp(unsigned int value, unsigned int mask);

extern "C" void __cdecl __setdefaultprecision() {
    _controlfp(0x10000, 0x30000);
}
