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

#if defined(_M_IX86) || defined(__i386__)
typedef char list_first_offset[(unsigned int)&(((List0043ebb0*)0)->first) == 0x208 ? 1 : -1];
typedef char node_next_offset[(unsigned int)&(((Node0043ebb0*)0)->next) == 4 ? 1 : -1];
typedef char node_link_offset[(unsigned int)&(((Node0043ebb0*)0)->list_link) == 0x0c ? 1 : -1];
#endif

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
