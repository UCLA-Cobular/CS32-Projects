//
// Created by jdc10 on 1/9/2021.
//

#ifndef PROJECT1_PLAYER_H
#define PROJECT1_PLAYER_H

class City;

class Player {
public:
    // Constructor
    Player(City *cp, int r, int c);

    // Accessors
    int row() const;

    int col() const;

    int age() const;

    int health() const;

    bool isPassedOut() const;

    // Mutators
    void preach();

    void move(int dir);

    void getGassed();

private:
    City *m_city;
    int m_row;
    int m_col;
    int m_health;
    int m_age;
};


#endif //PROJECT1_PLAYER_H
