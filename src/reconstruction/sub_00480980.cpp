// sub_00480980: EXE 0x00480980, 76 bytes. __stdcall with two stack
// arguments (RET 8); ECX/EDX are never read.
//
// Case-insensitive name lookup over a singly linked list. Walks the link at
// +0x00, comparing the caller's name against the string at record+0x14 where
// the record pointer sits at node+0x08 (MSVCR71 _stricmp, IAT 0x0070c624,
// loaded once into EBX before the loop). A hit returns the node itself when
// its +0x0c word is nonzero, otherwise the substitute pointer at +0x04; a
// exhausted walk returns null. Callers: 0x00480c80 and 0x00480d20.

extern "C" __declspec(dllimport) int __cdecl _stricmp(const char* string1,
                                                      const char* string2);

struct Node00480980 {
    Node00480980* field_00;
    void* field_04;
    char* field_08;
    unsigned int field_0c;
};

extern "C" void* __stdcall sub_00480980(const char* name, Node00480980* node) {
    while (node != 0) {
        if (_stricmp(name, node->field_08 + 0x14) == 0) {
            if (node->field_0c != 0) {
                return node;
            }
            return node->field_04;
        }
        node = node->field_00;
    }
    return 0;
}
