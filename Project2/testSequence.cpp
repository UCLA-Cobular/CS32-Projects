
// Allow easy switching between long and string modes
#define LONG
//#define STRING

#include "Sequence.h"
#include "testScoreList.cpp"
#include "testnewSequence.cpp"
#include <cassert>
#include <string>
#include <iostream>


using namespace std;

int main() {
#ifdef LONG
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

    testScoreList();

#endif

#ifdef STRING
    Sequence s1;
    s1.insert(0, "a");
    s1.insert(1, "b");
    s1.insert(2, "c");
    s1.insert(3, "b");
    s1.insert(4, "e");
    assert(s1.remove("b") == 2);
    assert(s1.size() == 3);
    string x;
    assert(s1.get(0, x) && x == "a");
    assert(s1.get(1, x) && x == "c");
    assert(s1.get(2, x) && x == "e");

    Sequence s4;
    s4.insert(0, "paratha");
    s4.insert(0, "focaccia");
    Sequence s2;
    s2.insert(0, "roti");
    s4.swap(s2);
    assert(s4.size() == 1 && s4.find("roti") == 0 && s2.size() == 2 &&
           s2.find("focaccia") == 0 && s2.find("paratha") == 1);

    Sequence s3;
    s3.insert(0, "dosa");
    s3.insert(1, "pita");
    s3.insert(2, "");
    s3.insert(3, "matzo");
    assert(s3.find("") == 2);
    s3.remove("dosa");
    assert(s3.size() == 3 && s3.find("pita") == 0 && s3.find("") == 1 &&
           s3.find("matzo") == 2);

    Sequence s;
    assert(s.empty());
    assert(s.find("laobing") == -1);
    assert(s.insert("laobing") == 0);
    assert(s.size() == 1  &&  s.find("laobing") == 0);

#endif
    cout << "Passed all tests" << endl;


    return 0;
}
