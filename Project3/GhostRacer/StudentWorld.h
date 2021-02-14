#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual      ~StudentWorld();
    virtual int  init();
    virtual int  move();
    virtual void cleanUp();

private:
    void    add_new_lines();
    void    initialize_lines();

	std::vector<Actor*> actorList;
    Actor* lastAddedWhiteLine;
};


#endif // STUDENTWORLD_H_
