//
// Created by jdc10 on 2/4/2021.
//

#include <string>
#include <cassert>

using namespace std;

class Coord {
public:
    Coord(int rr, int sc) : m_row(rr), m_col(sc) {}

    int r() const { return m_row; }

    int c() const { return m_col; }

private:
    int m_row;
    int m_col;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

int main() {
    {
        string maze[10] = {
                "XXXXXXXXXX",
                "X........X",
                "XX.X.XXXXX",
                "X..X.X...X",
                "X..X...X.X",
                "XXXX.XXX.X",
                "X.X....XXX",
                "X..XX.XX.X",
                "X...X....X",
                "XXXXXXXXXX"
        };
        assert(pathExists(maze, 10, 10, 6, 4, 1, 1));
        assert(!pathExists(maze, 10, 10, 9, 9, 1, 1));
    }
}


bool pathExists(string *maze, int nRows, int nCols, int sr, int sc, int er, int ec) {
    if (sr == er && sc == ec) return true;
    (maze + sr)->at(sc) = '#';
    // Check north
    if (sr - 1 > 0 && (maze + (sr - 1))->at(sc) == '.') {
        if(pathExists(maze, nRows, nCols, sr - 1, sc, er, ec)) {
            return true;
        }
    }
    // Check east
    if (sc + 1 < nCols && (maze + sr)->at(sc + 1) == '.') {
        if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec)){
            return true;
        }
    }
    // Check south
    if (sr + 1 < nRows && (maze + (sr + 1))->at(sc) == '.') {
        if(pathExists(maze, nRows, nCols, sr + 1, sc, er, ec)) {
            return true;
        }
    }
    // Check west
    if (sc - 1 > 0 && (maze + sr)->at(sc - 1) == '.') {
        if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec)) {
            return true;
        }
    }
    return false;
}



