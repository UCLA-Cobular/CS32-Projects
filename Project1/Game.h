//
// Created by jdc10 on 1/9/2021.
//

#ifndef PROJECT1_GAME_H
#define PROJECT1_GAME_H

#include "City.h"
#include "History.h"

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class City;

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nFlatulans);
    ~Game();

    // Mutators
    void play();

private:
    City* m_city;
};

int decodeDirection(char dir);

#endif //PROJECT1_GAME_H
