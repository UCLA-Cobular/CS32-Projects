#include <iostream>
#include <cassert>
#include "Sequence.h"

int main() {
    Sequence ss;  // ItemType is std::string
    assert(ss.size() == 0 && ss.empty());  // Test size and empty work on an empty Sequence
    ss.insert(0, "aaa");
    assert(ss.size() == 1 && !ss.empty());  // Test size and empty work on a filled Sequence
    ss.insert(1, "bbb");
    assert(ss.size() == 2 && !ss.empty());  // Test size and empty work on a filled Sequence
    ss.insert(2, "ccc");
    assert(ss.size() == 3 && !ss.empty());  // Test size and empty work on a filled Sequence
    ItemType x = "xxx";
    assert(!ss.get(3, x) && x == "xxx");  // x is unchanged
    assert(ss.get(1, x) && x == "bbb");  // x is set to bbb
    assert(ss.insert(4, "ddd") == -1 && ss.insert(-1, "ddd"));  // adding item at invalid pos fails
    assert(ss.insert(0, "zzz") == 0 && ss.get(0, x) && x == "zzz");  // zzz is inserted at start
    assert(ss.insert(1, "yyy") == 1 && ss.get(1, x) && x == "yyy");  // yyy is inserted in middle
    assert(ss.size() == 5 && !ss.empty());  // Test size and empty work on a filled Sequence

    // Array currently: zzz, yyy, aaa, bbb, ccc
    assert(ss.erase(1) && ss.get(0, x) && x == "zzz" && ss.get(1, x) && x == "aaa");  // Test that erase works in middle
    assert(ss.erase(0) && ss.get(0, x) && x == "aaa" && ss.get(1, x) && x == "bbb");  // Test that erase works at start
    assert(ss.erase(2) && !ss.get(2, x) && ss.get(1, x) && x == "bbb");  // Test that erase works at end
    assert(!ss.erase(-1) && !ss.erase(2) && ss.get(0, x) && x == "aaa" && ss.get(1, x) &&
           x == "bbb");  // Test that fails with bad inputs and leaves everything unchanged
    assert(ss.size() == 2 && !ss.empty());  // Test size and empty work on a filled Sequence

    ss.insert(0, "fff");
    ss.insert(0, "ggg");
    ss.insert(0, "hhh");

    // List currently: hhh, ggg, fff, aaa, bbb
    assert(ss.set(0, "vvv") && ss.get(0, x) && x == "vvv");  // Test that set works
    assert(ss.set(2, "lll") && ss.get(2, x) && x == "lll");  // Test that set works
    assert(ss.set(4, "ooo") && ss.get(4, x) && x == "ooo");  // Test that set works
    assert(ss.get(0, x) && x == "vvv" && ss.get(1, x) && x == "ggg" && ss.get(2, x) && x == "lll" && ss.get(3, x) &&
           x == "aaa" && ss.get(4, x) && x == "ooo" && !ss.get(5, x) &&
           x == "ooo");  // Test that set didn't do anything funky to the list
    assert(!ss.set(-1, "ppp") && !ss.set(5, "ppp")); // Test that set fails when it should
    assert(ss.get(0, x) && x == "vvv" && ss.get(1, x) && x == "ggg" && ss.get(2, x) && x == "lll" &&
           ss.get(3, x) && x == "aaa" && ss.get(4, x) && x == "ooo" && !ss.get(5, x) &&
           x == "ooo");  // Test that set didn't do anything funky to the list


    std::cerr << "tests fin" << std::endl;
    return 0;
}
