// sub_00422180: EXE 0x00422180, 22 bytes. MSVC 7.1 (/O2 /Zl).
//
// Stride-or-table index resolver: __fastcall with the record in ECX and the
// index in EDX (mangled @sub_00422180@8). When the mode word at +0 is zero,
// the record is (0, base +4, stride +8) and the result is stride*index +
// base; otherwise the dword at +4 is a pointer table and the result is
// table[index]. sub_00423d50 inlines this exact shape twice (0x00423f56 and
// 0x00423f66 region) for records reached through the container at 0x016e3050.
// Called from sub_00422730 at 0x00422938 and 0x004229ed.

extern "C" int __fastcall sub_00422180(void* self, int index) {
    if (*(int*)self == 0) {
        return ((int*)self)[2] * index + ((int*)self)[1];
    }
    return ((int**)self)[1][index];
}
