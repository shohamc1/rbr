// Build flags: /arch:SSE

// sub_00406370: MSVC 7.1 (/O2 /Zl /arch:SSE).
//
// __cdecl cross product of two three-float vectors into a caller-provided
// destination, which is returned in EAX:
//   out.x = right.z * left.y  - left.z * right.y
//   out.y = left.z * right.x  - right.z * left.x
//   out.z = left.x * right.y  - right.x * left.y
// Each component is computed in XMM0, spilled to a 12-byte local on the
// stack (SUB ESP,0xc), and the local is then copied over the destination
// with three dword moves (the MSVC 12-byte POD assignment shape), interleaved
// with the remaining component arithmetic by the scheduler. Operand order
// inside each product mirrors the reference loads.

struct Vector3_00406370 {
    float field_0[3];
};

// FUNCTION: RBR 0x00406370
extern "C" void* __cdecl sub_00406370(Vector3_00406370* destination, const Vector3_00406370* left,
                                      const Vector3_00406370* right) {
    Vector3_00406370 result;
    result.field_0[0] = right->field_0[2] * left->field_0[1] - left->field_0[2] * right->field_0[1];
    result.field_0[1] = left->field_0[2] * right->field_0[0] - right->field_0[2] * left->field_0[0];
    result.field_0[2] = left->field_0[0] * right->field_0[1] - right->field_0[0] * left->field_0[1];
    *destination = result;
    return destination;
}
