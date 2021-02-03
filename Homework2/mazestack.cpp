#include <iostream>
#include <string>
#include <stack>
#include <cassert>

using namespace std;

class Coord {
public:
    Coord(int rr, int cc) : m_row(rr), m_col(cc) {}

    int r() const { return m_row; }

    int c() const { return m_col; }

private:
    int m_row;
    int m_col;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
void markMazeCoord(int row, int col, string *maze);
char getMazeCoord(int row, int col, const string *maze);
Coord pop(stack<Coord> &stack);
bool compare(const Coord &c1, const Coord &c2);
bool checkMazeCoord(int row, int col, string *maze);

// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

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
        assert(getMazeCoord(0, 0, maze) == 'X' && getMazeCoord(1, 1, maze) == '.' && getMazeCoord(4, 2, maze) == '.');
        markMazeCoord(0, 0, maze);
        markMazeCoord(1, 1, maze);
        markMazeCoord(4, 2, maze);
        assert(getMazeCoord(0, 0, maze) == '#' && getMazeCoord(1, 1, maze) == '#' && getMazeCoord(4, 2, maze) == '#');
    }
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

    return 0;
}

bool pathExists(string *maze, int nRows, int nCols, int sr, int sc, int er, int ec) {
    stack<Coord> coordStack;

    markMazeCoord(sr, sc, maze);
    coordStack.push(Coord(sr, sc));

    Coord final_coord = Coord(er, ec);

    while (!coordStack.empty()) {
        Coord currentCoord = pop(coordStack);

        // Return true on success
        if (compare(currentCoord, final_coord)) return true;
        // Setup current row and current col
        const int rc = currentCoord.r();
        const int cc = currentCoord.c();

        // Check north
        if (rc - 1 > 0 && getMazeCoord(rc - 1, cc, maze) == '.') {
            markMazeCoord(rc - 1, cc, maze);
            coordStack.push(Coord(rc - 1, cc));
        }
        // Check east
        if (cc + 1 < nCols && getMazeCoord(rc, cc + 1, maze) == '.') {
            markMazeCoord(rc, cc + 1, maze);
            coordStack.push(Coord(rc, cc + 1));
        }
        // Check south
        if (rc + 1 < nRows && getMazeCoord(rc + 1, cc, maze) == '.') {
            markMazeCoord(rc + 1, cc, maze);
            coordStack.push(Coord(rc + 1, cc));
        }
        // Check west
        if (cc - 1 > 0 && getMazeCoord(rc, cc - 1, maze) == '.') {
            markMazeCoord(rc, cc - 1, maze);
            coordStack.push(Coord(rc, cc - 1));
        }
    }
    // No solution, return false
    return false;
}


char getMazeCoord(int row, int col, const string *maze) {
    return (maze + row)->at(col);
}

void markMazeCoord(int row, int col, string *maze) {
    (maze + row)->at(col) = '#';
}


Coord pop(stack<Coord> &stack) {
    Coord currentCoord = stack.top();
    stack.pop();
    return currentCoord;
}

bool compare(const Coord &c1, const Coord &c2) {
    return c1.c() == c2.c() && c1.r() == c2.r();
}

bool checkMazeCoord(int row, int col, string *maze) {
    return (maze + row)->at(col) == '#';
}

