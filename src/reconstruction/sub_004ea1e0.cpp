// sub_004ea1e0: EXE 0x004ea1e0, 15 bytes. __thiscall member, no stack
// arguments: returns (this[0x68a] >> 2) & 1 as a full EAX value.
// MSVC 7.1 widens the unsigned char with XOR EAX,EAX + MOV AL (not MOVZX).
class Object004ea1e0 {
public:
    unsigned int sub_004ea1e0();

private:
    unsigned char field_00[0x68b];
};

unsigned int Object004ea1e0::sub_004ea1e0()
{
    return (field_00[0x68a] >> 2) & 1;
}
