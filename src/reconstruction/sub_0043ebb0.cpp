#include "layout.h"
struct Node0043ebb0 {
    unsigned int unknown_00;
    Node0043ebb0* next;
    Node0043ebb0* previous;
    void* list_link;
};

struct List0043ebb0 {
    unsigned char unknown_00[0x208];
    Node0043ebb0* first;
    Node0043ebb0* last;
    unsigned int count;
    void sub_0043ebb0(void* node);
};

LAYOUT_OFFSET(List0043ebb0, first, 0x208);
LAYOUT_OFFSET(Node0043ebb0, next, 4);
LAYOUT_OFFSET(Node0043ebb0, list_link, 0x0c);

// NONMATCHING
// FUNCTION: RBR 0x0043ebb0
void List0043ebb0::sub_0043ebb0(void* address) {
    Node0043ebb0* node = (Node0043ebb0*)address;
    node->previous = last;
    node->next = 0;
    if (last != 0) {
        last->next = node;
        last = node;
        node->list_link = &first;
        ++count;
        return;
    }
    first = node;
    last = node;
    node->list_link = &first;
    ++count;
}
