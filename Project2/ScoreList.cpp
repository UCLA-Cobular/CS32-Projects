//
// Created by cover on 1/14/2021.
//

#include "ScoreList.h"
#include "Sequence.h"

ScoreList::ScoreList() {
    m_sequence = Sequence();
}

bool ScoreList::add(unsigned long score) {
    if (score >= 0 && score <= 100 && m_sequence.size() < DEFAULT_MAX_ITEMS)
        return false;
//    m_sequence.insert(score);
    return true;
}

bool ScoreList::remove(unsigned long score) {
    return false;
}

unsigned long ScoreList::minimum() const {
    return 0;
}

unsigned long ScoreList::maximum() const {
    return 0;
}
