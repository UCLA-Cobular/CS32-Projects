//
// Created by jdc10 on 1/9/2021.
//

#include "History.h"
#include "Flatulan.h"
#include <iostream>

History::History(int nRows, int nCols) : m_nRows(nRows), m_nCols(nCols) {
    for (int r = 0; r < nRows; r++)
        for (int c = 0; c < nCols; c++)
            history_data[r][c] = '.';
}

bool History::record(int r, int c) {
    if (r > m_nRows || c > m_nCols || r < 1 || c < 1)
        return false;

    char *historyDatapoint = &history_data[r - 1][c - 1];

    if (*historyDatapoint == '.')
        *historyDatapoint = 'A';
    else if (*historyDatapoint >= 'A' && *historyDatapoint < 'Z') { (*historyDatapoint)++; };

    return true;
}

void History::display() const {
    // Draw the grid
    clearScreen();
    for (int r = 0; r < m_nRows; r++) {
        for (int c = 0; c < m_nCols; c++)
            std::cout << history_data[r][c];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
