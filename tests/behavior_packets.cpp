#include <assert.h>

#include "../src/reconstruction/sub_00416390.cpp"
#include "../src/reconstruction/sub_004163c0.cpp"
#include "../src/reconstruction/sub_0043ebb0.cpp"
#include "../src/reconstruction/sub_0043d500.cpp"
#include "../src/reconstruction/sub_004183d0.cpp"

int main() {
    Record00416390 recordsA[4] = {};
    Collection00416390 a = {};
    a.records = recordsA;
    a.maximum_index = 2;
    a.sub_00416390(1, 11);
    assert(recordsA[1].field_00 == 11 && a.maximum_index == 2);
    a.sub_00416390(2, 22);
    assert(recordsA[2].field_00 == 22 && a.maximum_index == 2);
    a.sub_00416390(3, 33);
    assert(recordsA[3].field_00 == 33 && a.maximum_index == 3);

    Record004163c0 recordsB[4] = {};
    Collection004163c0 b = {};
    b.records = recordsB;
    b.maximum_index = 2;
    b.sub_004163c0(1, 101);
    assert(recordsB[1].field_10 == 101 && b.maximum_index == 2);
    b.sub_004163c0(2, 202);
    assert(recordsB[2].field_10 == 202 && b.maximum_index == 2);
    b.sub_004163c0(3, 303);
    assert(recordsB[3].field_10 == 303 && b.maximum_index == 3);

    List0043ebb0 list = {};
    Node0043ebb0 first = {}, second = {};
    list.sub_0043ebb0(&first);
    assert(list.first == &first && list.last == &first && list.count == 1);
    assert(first.next == 0 && first.previous == 0 && first.list_link == &list.first);
    list.sub_0043ebb0(&second);
    assert(list.first == &first && list.last == &second && list.count == 2);
    assert(second.previous == &first && second.next == 0);
    assert(first.next == &second && second.list_link == &list.first);

    unsigned int stationary[] = {7, 8};
    unsigned int copied[6] = {};
    sub_0043d500(copied, copied, stationary);
    for (int i = 0; i < 6; ++i) assert(copied[i] == 0);
    sub_0043d500(copied, copied + 6, stationary);
    assert(copied[0] == 7 && copied[1] == 8);
    assert(copied[2] == 7 && copied[3] == 8);
    assert(copied[4] == 7 && copied[5] == 8);

    const unsigned int words[] = {10, 20, 30, 40, 50};
    Reader004183d0 reader = {words, 4};
    unsigned int output[3] = {};
    reader.sub_004183d0(output);
    assert(output[0] == 20 && output[1] == 30 && output[2] == 40);
    assert(reader.offset == 16);
    const unsigned int nextWords[] = {60, 70, 80};
    reader.data = nextWords;
    reader.offset = 0;
    reader.sub_004183d0(output);
    assert(output[0] == 60 && output[1] == 70 && output[2] == 80);
    assert(reader.offset == 12);
}
