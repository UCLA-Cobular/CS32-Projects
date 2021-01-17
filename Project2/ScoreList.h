//
// Created by cover on 1/14/2021.
//

#ifndef PROJECT2_SCORELIST_H
#define PROJECT2_SCORELIST_H


#include "newSequence.h"
#include <limits>

const unsigned long NO_SCORE = std::numeric_limits<unsigned long>::max();

class ScoreList
{
public:
    ScoreList();       // Create an empty score list.

    bool add(unsigned long score);
    // If the score is valid (a value from 0 to 100) and the score list
    // has room for it, add it to the score list and return true.
    // Otherwise, leave the score list unchanged and return false.

    bool remove(unsigned long score);
    // Remove one instance of the specified score from the score list.
    // Return true if a score was removed; otherwise false.

    int size() const {
        return m_sequence.size();
    }
    // Return the number of scores in the list.

    unsigned long minimum() const;
    // Return the lowest-valued score in the score list.  If the list is
    // empty, return NO_SCORE.

    unsigned long maximum() const;
    // Return the highest-valued score in the score list.  If the list is
    // empty, return NO_SCORE.

private:
    // Some of your code goes here.
    Sequence m_sequence;
};


#endif //PROJECT2_SCORELIST_H
