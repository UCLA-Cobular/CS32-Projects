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
	void    updateDisplayText();
	void    saveSoul() { m_souls_2_save--; }

	GhostRacer* ghost_racer() const { return m_ghost_racer; }

private:
	void add_holy_water();
	void add_new_lines();
	void initialize_lines();

	std::vector<Actor*> actorList;
	GhostRacer*         m_ghost_racer;
	int                 m_souls_2_save;
	int                 m_bonus_pts;
	double              unitsSinceLastAddedWhiteLine;
};


#endif // STUDENTWORLD_H_
