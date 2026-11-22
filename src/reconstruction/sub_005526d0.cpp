// sub_005526d0: EXE 0x005526d0, 31 bytes. __cdecl callback, no arguments,
// registered at EXE 0x00552d5b (pushed with (0, 1, callback) into sub_005726e0).
// Sets the two name-format string fields of the fixed object at EXE 0x007b184c:
// "qr%02d_in.arp" (0x00726f3c) through sub_0056f250 (stores at +0x0c) and
// "qr%02d_out.arp" (0x00726f2c) through sub_0056f260 (stores at +0x10). Both
// callees are __thiscall members with one cdecl stack argument (RET 4). The
// sibling clear member at 0x0056f200 zeroes +0x00..+0x10 with -1 at +0x04.
// Compiled with MSVC 7.1 (/O2 /Zl).

// Callee class declaration; both callees have their own manifest entries.
class Object0056f250 {
public:
    void sub_0056f250(const char *name);
    void sub_0056f260(const char *name);
};

extern "C" Object0056f250 gObject007b184c; // EXE .data 0x007b184c

extern "C" void __cdecl sub_005526d0() {
    gObject007b184c.sub_0056f250("qr%02d_in.arp");
    gObject007b184c.sub_0056f260("qr%02d_out.arp");
}
