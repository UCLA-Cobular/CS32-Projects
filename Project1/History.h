//
// Created by jdc10 on 1/9/2021.
//

#ifndef PROJECT1_HISTORY_H
#define PROJECT1_HISTORY_H

#include "globals.h"

class History {
public:
    History(int nRows, int nCols);

    bool record(int r, int c);

    void display() const;

private:
    int m_nRows;
    int m_nCols;

    char history_data[MAXROWS][MAXCOLS]{};

};


#endif //PROJECT1_HISTORY_H
