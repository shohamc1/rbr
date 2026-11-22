// sub_0043a290: EXE 0x0043a290, 21 bytes. Compiled with MSVC 7.1 (/O2 /Zl).
//
// Allocation helper for 4-byte elements: operator new(count * 4), returning
// the block. sub_006efa32 is the pinned operator new (malloc/_callnewh/
// std::_Nomemory retry loop). Pairs with sub_0043a200, which hands the
// block back through the operator-delete thunk sub_006efdd0. cdecl with a
// second unused argument: a tail jump into sub_006efa32 is impossible while
// the caller frame carries an argument nobody cleans, and the reference
// keeps PUSH/CALL/ADD ESP,4/RET. The (count, hint) shape matches the
// allocator family of sub_0043a200 (two arguments, RET 8).
extern "C" void* __cdecl sub_006efa32(unsigned int size);   // operator new

extern "C" void* __cdecl sub_0043a290(unsigned int count, const void*) {
    return sub_006efa32(count * 4);
}
