#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class GhostRacer;


class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual ~StudentWorld();
	int     init() override;
	int     move() override;
	void    cleanUp() override;
	void    updateUnitsSinceLastAddedWhiteLine();

	GhostRacer* ghost_racer() const { return m_ghost_racer; }
private:
	void add_new_lines();
	void initialize_lines();

	std::vector<Actor*> actorList;
	GhostRacer*         m_ghost_racer;
	double              unitsSinceLastAddedWhiteLine;
};


#endif // STUDENTWORLD_H_
