// sub_00499570: EXE 0x00499570, 35 bytes. __thiscall, one stack word (RET 4).
// field_00 is the buffer end, field_04 the write cursor. When
// cursor + count would pass the end, zero-fills the overflow amount byte by
// byte and advances the cursor. The byte store may alias the cursor field, so
// both statements reload it each iteration.

struct Object00499570 {
    char* field_00;
    char* field_04;
    void sub_00499570(int count);
};

void Object00499570::sub_00499570(int count)
{
    int overflow = (field_00 - field_04) + count;
    if (overflow > 0) {
        do {
            *field_04 = 0;
            field_04 = field_04 + 1;
        } while (--overflow);
    }
}
