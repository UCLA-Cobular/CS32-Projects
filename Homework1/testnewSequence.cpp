//
// Created by cover on 1/14/2021.
//

#include "newSequence.h"
#include <cassert>
#include <iostream>

#define LONG

using namespace std;

int main() {
    Sequence a(1000);   // a can hold at most 1000 items
    Sequence b(5);      // b can hold at most 5 items
    Sequence c;         // c can hold at most DEFAULT_MAX_ITEMS items
    ItemType v = 40;

    // No failures inserting 5 items into b
    for (int k = 0; k < 5; k++)
        assert(b.insert(v) != -1);

    // Failure if we try to insert a sixth item into b
    assert(b.insert(v) == -1);

    // When two Sequences' contents are swapped, their capacities are
    // swapped as well:
    a.swap(b);
    assert(a.insert(v) == -1  &&  b.insert(v) != -1);

    Sequence s;
    assert(s.empty());
    assert(s.find(42) == -1);
    assert(s.insert(42) == 0);
    assert(s.size() == 1 && s.find(42) == 0);
    assert(s.insert(41) == 0);
    assert(s.insert(32) == 0);
    assert(s.insert(100) == 3);
    assert(s.size() == 4 && s.find(100) == 3);
    assert(s.insert(0, 12) == 0 && s.find(12) == 0);
    assert(s.insert(2, 33) && s.find(33) == 2);
    assert(s.size() == 6);
    // Array currently: 12|32|33|41|42|100
    // s.dump();
    assert(s.erase(0) == true && s.size() == 5 && s.find(32) == 0 && s.find(12) == -1);
    assert(s.erase(3) == true && s.size() == 4 && s.find(100) == 3 && s.find(42) == -1);
    assert(s.erase(5) == false && s.erase(-1) == false);
    // Array currently: 32|33|41|100
    assert(s.insert(100) == 3 && s.insert(100) == 3 && s.insert(100) == 3 && s.size() == 7);
    // Array currently: 32|33|41|100|100|100|100
    assert(s.remove(100) == 4 && s.size() == 3);
    assert(s.remove(100) == 0 && s.size() == 3);
    // Array currently: 32|33|41
    ItemType item;
    assert(s.get(0, item) == true && item == 32);
    assert(s.get(2, item) == true && item == 41);
    assert(s.get(3, item) == false && item == 41);
    assert(s.get(-1, item) == false && s.get(100, item) == false && item == 41);
    // Array currently: 32|33|41
    assert(s.set(0, 31) == true && s.get(0, item) == true && item == 31 && s.find(32) == -1);
    assert(s.set(-1, 44) == false && s.set(4, 44) == false && s.find(44) == -1);
    s.insert(55);
    s.insert(43);
    s.insert(22);
    s.insert(1);
    s.insert(83);
    // Array currently: 1|22|31|33|41|43|55|83
    Sequence s2;
    s2.insert(37);
    s2.insert(11);
    s2.insert(76);
    s2.insert(13);
    s2.insert(48);
    // Array currently: 11|13|37|48|76
    s.swap(s2);
    assert(s.size() == 5 && s.find(11) == 0 && s.find(13) == 1 && s.find(37) == 2 && s.find(48) == 3 &&
           s.find(76) == 4);
    assert(s2.size() == 8 && s2.find(1) == 0 && s2.find(22) == 1 && s2.find(31) == 2 && s2.find(33) == 3 &&
           s2.find(41) == 4 && s2.find(43) == 5 && s2.find(55) == 6 && s2.find(83) == 7);
//    s2.dump();

    cerr << "All tests done" << endl;
}
