// Object004b0400 constructor: EXE 0x004b0400, 13 bytes. __thiscall with
// ECX-in/EAX-out (the MOV EAX,ECX prologue identifies a constructor).
// Zeroes three dword fields at +0, +4, +8; the +4 store is emitted first.
// No direct callers in Ghidra; consistent with vtable or data dispatch.
// Hypothesis for the 0x004b04xx family: an intrusive list header with
// head +0, tail +4, count +8 (tail-append sub_004b04b0 links those fields
// on nodes with next +0, prev +4, owner +8).

struct Object004b0400 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    Object004b0400();
};

// Object004b0420 constructor: EXE 0x004b0420, 13 bytes. __thiscall with
// ECX-in/EAX-out (MOV EAX,ECX prologue identifies a constructor).
// Byte-identical to the Object004b0400 constructor at 0x004b0400: zeroes
// the dword fields at +0, +4, +8 with the +4 store emitted first.
// Same 0x004b04xx intrusive-list family hypothesis (head +0, tail +4,
// count +8); a separate emitted body implies a separate class or
// translation unit for the original build.

struct Object004b0420 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    Object004b0420();
};

// Object004b0460 member: EXE 0x004b0460, 17 bytes. __thiscall with two
// dword stack arguments (RET 8), ECX receiver, void return.
// Stores them into the fields at +0x10 and +0x14 in argument order. The
// receiver holds at least 0x18 bytes; the first 0x10 bytes match the
// 16-byte list-header family constructed by sub_004b04a0/sub_004b04e0,
// so this is plausibly a pair setter on a class embedding that header.

struct Object004b0460 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
    unsigned int field_10;
    unsigned int field_14;
    void sub_004b0460(unsigned int a, unsigned int b);
};

// Object004b0490 constructor: EXE 0x004b0490, 13 bytes. __thiscall with
// ECX-in/EAX-out (MOV EAX,ECX prologue identifies a constructor).
// Byte-identical to the Object004b0400 constructor at 0x004b0400: zeroes
// the dword fields at +0, +4, +8 with the +4 store emitted first.
// Same 0x004b04xx intrusive-list family hypothesis (head +0, tail +4,
// count +8); a separate emitted body implies a separate class or
// translation unit for the original build.

struct Object004b0490 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    Object004b0490();
};

// Object004b04a0 constructor: EXE 0x004b04a0, 16 bytes. __thiscall with
// ECX-in/EAX-out (MOV EAX,ECX prologue identifies a constructor).
// Zeroes four dword fields at +0, +4, +8, +0xc; the +4 store is emitted
// first. Hypothesis for the 0x004b04xx family: this is the 16-byte list
// header of the prefixed-node family whose tail-append sub_004b0520
// links nodes with next +4, prev +8, owner +0xc (head +0, tail +4,
// count +8, spare +0xc).

struct Object004b04a0 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
    Object004b04a0();
};

// Object004b04b0 member: EXE 0x004b04b0, 48 bytes. __thiscall with one
// pointer stack argument (RET 4), void return.
// Tail-append for an intrusive doubly linked list. List header: first
// node +0, last node +4, count +8 (zeroed by the 0x004b04a0-family
// constructors). Node: next +0, prev +4, owner +8. The new node's
// prev/next stores are hoisted above the branch (prev takes the old
// last, which is zero in the empty case); MSVC tail-duplicates the
// owner/count/RET suffix into both paths.
// Byte-identical to sub_004b04f0 at 0x004b04f0.

struct Node004b04b0 {
    Node004b04b0 *field_00_next;
    Node004b04b0 *field_04_prev;
    void *field_08_owner;
};

struct Object004b04b0 {
    Node004b04b0 *field_00_first;
    Node004b04b0 *field_04_last;
    unsigned int field_08_count;
    void sub_004b04b0(Node004b04b0 *node);
};

// Object004b04e0 constructor: EXE 0x004b04e0, 16 bytes. __thiscall with
// ECX-in/EAX-out (MOV EAX,ECX prologue identifies a constructor).
// Byte-identical to the Object004b04a0 constructor at 0x004b04a0: zeroes
// four dword fields at +0, +4, +8, +0xc with the +4 store emitted first.
// Same 16-byte list-header hypothesis as sub_004b04a0; a separate emitted
// body implies a separate class or translation unit for the original
// build.

struct Object004b04e0 {
    unsigned int field_00;
    unsigned int field_04;
    unsigned int field_08;
    unsigned int field_0c;
    Object004b04e0();
};

// Object004b04f0 member: EXE 0x004b04f0, 48 bytes. __thiscall with one
// pointer stack argument (RET 4), void return.
// Byte-identical tail-append to sub_004b04b0 at 0x004b04b0: intrusive
// doubly linked list with first +0, last +4, count +8 and nodes holding
// next +0, prev +4, owner +8. A separate emitted body implies a separate
// class or translation unit for the original build.

struct Node004b04f0 {
    Node004b04f0 *field_00_next;
    Node004b04f0 *field_04_prev;
    void *field_08_owner;
};

struct Object004b04f0 {
    Node004b04f0 *field_00_first;
    Node004b04f0 *field_04_last;
    unsigned int field_08_count;
    void sub_004b04f0(Node004b04f0 *node);
};

// Object004b0520 member: EXE 0x004b0520, 50 bytes. __thiscall with one
// pointer stack argument (RET 4), void return.
// Tail-append for the prefixed-node intrusive list family: the list
// header keeps first +0, last +4, count +8 (16-byte header constructed
// by sub_004b04a0/sub_004b04e0), while nodes carry a dword prefix at +0
// before the link block: next +4, prev +8, owner +0xc — the same node
// shape as the wave-1 intrusive-list wrapper family. Two bytes longer
// than the bare-node sub_004b04b0 because the larger displacements
// (+0xc owner, [EDX+4] next) need full encodings.

struct Node004b0520 {
    unsigned int field_00;
    Node004b0520 *field_04_next;
    Node004b0520 *field_08_prev;
    void *field_0c_owner;
};

struct Object004b0520 {
    Node004b0520 *field_00_first;
    Node004b0520 *field_04_last;
    unsigned int field_08_count;
    void sub_004b0520(Node004b0520 *node);
};

// FUNCTION: RBR 0x004b0400
Object004b0400::Object004b0400() {
    field_04 = 0;
    field_00 = 0;
    field_08 = 0;
}

// FUNCTION: RBR 0x004b0420
Object004b0420::Object004b0420() {
    field_04 = 0;
    field_00 = 0;
    field_08 = 0;
}

// FUNCTION: RBR 0x004b0460
void Object004b0460::sub_004b0460(unsigned int a, unsigned int b) {
    field_10 = a;
    field_14 = b;
}

// FUNCTION: RBR 0x004b0490
Object004b0490::Object004b0490() {
    field_04 = 0;
    field_00 = 0;
    field_08 = 0;
}

// FUNCTION: RBR 0x004b04a0
Object004b04a0::Object004b04a0() {
    field_04 = 0;
    field_00 = 0;
    field_08 = 0;
    field_0c = 0;
}

// FUNCTION: RBR 0x004b04b0
void Object004b04b0::sub_004b04b0(Node004b04b0 *node) {
    Node004b04b0 *last = field_04_last;
    node->field_04_prev = last;
    node->field_00_next = 0;
    if (last != 0) {
        last->field_00_next = node;
        field_04_last = node;
    } else {
        field_00_first = node;
        field_04_last = node;
    }
    node->field_08_owner = this;
    ++field_08_count;
}

// FUNCTION: RBR 0x004b04e0
Object004b04e0::Object004b04e0() {
    field_04 = 0;
    field_00 = 0;
    field_08 = 0;
    field_0c = 0;
}

// FUNCTION: RBR 0x004b04f0
void Object004b04f0::sub_004b04f0(Node004b04f0 *node) {
    Node004b04f0 *last = field_04_last;
    node->field_04_prev = last;
    node->field_00_next = 0;
    if (last != 0) {
        last->field_00_next = node;
        field_04_last = node;
    } else {
        field_00_first = node;
        field_04_last = node;
    }
    node->field_08_owner = this;
    ++field_08_count;
}

// FUNCTION: RBR 0x004b0520
void Object004b0520::sub_004b0520(Node004b0520 *node) {
    Node004b0520 *last = field_04_last;
    node->field_08_prev = last;
    node->field_04_next = 0;
    if (last != 0) {
        last->field_04_next = node;
        field_04_last = node;
    } else {
        field_00_first = node;
        field_04_last = node;
    }
    node->field_0c_owner = this;
    ++field_08_count;
}
