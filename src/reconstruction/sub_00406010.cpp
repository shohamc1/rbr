// sub_00406010: EXE 0x00406010, 48 bytes. MSVC 7.1 (/O2 /Zl).
//
// Dispatch loop: calls the same __thiscall member on consecutive elements of
// a stride-addressed array. Arguments: first element, byte stride, element
// count, and the member pointer. The loop is rotated (guard "dec; js" skips
// nonpositive counts), the element pointer is strength-reduced with ADD, and
// the 2-byte "lea ebx,[ebx]" pads the loop head to a 16-byte boundary.
// The element class is non-polymorphic so the member pointer is a thin code
// pointer: the call is "mov ecx, element; call method". __stdcall, RET 0x10.
//
// Callers include sub_00480af0, sub_005c5770, sub_00476810, sub_00474ae0 and
// sub_00494140-vicinity game loops.

class Element00406010 {
public:
    void member00406010();
};

extern "C" void __stdcall sub_00406010(Element00406010* element, int stride, int count,
                                       void (Element00406010::*method)()) {
    while (--count >= 0) {
        (element->*method)();
        element = (Element00406010*)((char*)element + stride);
    }
}
