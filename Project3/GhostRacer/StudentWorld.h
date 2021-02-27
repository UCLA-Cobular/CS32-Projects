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
	virtual       ~StudentWorld();
	int           init() override;
	int           move() override;
	void          cleanUp() override;
	void          updateDisplayText();
	void          saveSoul() { m_souls_2_save--; }
	void          shortCircuitEndLevel() { m_short_circuit_end = true; }
	void          addHealthPack(double startX, double startY);
	void          addHolyWaterSpray(double startX, double startY, int startDir);
	void          add_oil_slick(double startX, double startY);
	double        collisionActorInLane(int lane, double y_coord, bool behind);
	Actor*        projectileCollision(double x_coord, double y_coord, double radius);
	static int    coordToLane(double x_coord);

	GhostRacer* ghost_racer() const { return m_ghost_racer; }

private:
	static int    random_x_value();
	static double laneToCoord(int lane);
	
	void          updateUnitsSinceLastAddedWhiteLine();
	bool          checkShortCircuitEnd() const { return m_short_circuit_end; }

	// Addition functions should be called every loop, as they have their own spawning chance logic internally.
	void add_holy_water();
	void add_human_peds();
	void add_zombie_peds();
	void add_oil_slick();
	void add_zombie_cab();
	void add_lost_soul();
	void add_new_lines();
	void initialize_lines();

	std::vector<Actor*> actorVector;
	GhostRacer*         m_ghost_racer;
	int                 m_souls_2_save;
	int                 m_bonus_pts;
	double              unitsSinceLastAddedWhiteLine;
	// Checked after every actor is processed, allows the actor to kill the level
	bool m_short_circuit_end;
};


#endif // STUDENTWORLD_H_
