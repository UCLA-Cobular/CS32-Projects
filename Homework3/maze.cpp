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



