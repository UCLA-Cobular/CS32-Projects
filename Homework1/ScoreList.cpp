//
// Created by cover on 1/14/2021.
//

#include "ScoreList.h"
//#include "Sequence.h"

ScoreList::ScoreList() {
    m_sequence = Sequence();
}

bool ScoreList::add(unsigned long score) {
    if (score < 0 || score > 100 || m_sequence.size() > DEFAULT_MAX_ITEMS)
        return false;
    if (m_sequence.insert(score) == -1) {
        return false;
    };
    return true;
}

bool ScoreList::remove(unsigned long score) {
    int p = m_sequence.find(score);
    if (p == -1)
        return false;
    if (!m_sequence.erase(p))
        return false;
    return true;
}

unsigned long ScoreList::minimum() const {
    if (m_sequence.empty()) return NO_SCORE;
    unsigned long ref = 0;
    m_sequence.get(0, ref);
    return ref;

//    Bad first implementation
//    unsigned long minimum = 100;
//    unsigned long comparison_holder = 0;
//    for (int i = 0; i < m_sequence.size(); ++i) {
//        m_sequence.get(i, comparison_holder);
//        if (minimum > comparison_holder) minimum = comparison_holder;
//    }
//    return minimum;
}

unsigned long ScoreList::maximum() const {
    if (m_sequence.empty()) return NO_SCORE;
    unsigned long ref = 0;
    m_sequence.get(m_sequence.size() - 1, ref);
    return ref;

//    Bad first implementation
//    unsigned long maximum = 0;
//    unsigned long comparison_holder = 0;
//    for (int i = 0; i < m_sequence.size(); ++i) {
//        m_sequence.get(i, comparison_holder);
//        if (maximum < comparison_holder) maximum = comparison_holder;
//    }
//    return maximum;
}
