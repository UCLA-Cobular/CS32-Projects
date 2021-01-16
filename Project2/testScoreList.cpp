//
// Created by cover on 1/14/2021.
//

#define PROJECT2_TESTSCORELIST_CPP
#ifdef PROJECT2_TESTSCORELIST_CPP


#include "ScoreList.h"
#include <cassert>

void testScoreList() {
    ScoreList scoreList1;
    assert(scoreList1.add(100) == true && scoreList1.add(80) == true && scoreList1.add(1) == true &&
           scoreList1.add(40) == true && scoreList1.add(50) == true && scoreList1.size() == 5);
    assert(scoreList1.add(140) == false && scoreList1.add(-1) == false && scoreList1.size() == 5);

    ScoreList scoreList2;
    for (int i = 0; i < 150; ++i) {
        assert(scoreList2.add(99) && scoreList2.size() == i + 1);
    }
    assert(scoreList2.add(99) == false && scoreList2.size() == 150);
    assert(scoreList2.remove(1) == false && scoreList2.size() == 150);
    assert(scoreList2.remove(99) == true && scoreList2.size() == 149);
    assert(scoreList2.remove(99) == true && scoreList2.size() == 148);
    assert(scoreList2.add(1) && scoreList2.size() == 149);
    assert(scoreList2.add(100) && scoreList2.size() == 150);
    assert(scoreList2.minimum() == 1);
    assert(scoreList2.maximum() == 100);

}

#endif // PROJECT2_TESTSCORELIST_CPP
