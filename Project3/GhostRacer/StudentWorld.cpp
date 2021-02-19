#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream
using namespace std;

GameWorld* createStudentWorld(string assetPath) { return new StudentWorld(assetPath); }

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath), m_ghost_racer(nullptr), unitsSinceLastAddedWhiteLine(0), m_souls_2_save(0),
	  m_bonus_pts(5000), m_short_circuit_end(false) {}

StudentWorld::~StudentWorld() { StudentWorld::cleanUp(); }

void StudentWorld::initialize_lines()
{
	for (int i = 0; i < VIEW_HEIGHT / SPRITE_HEIGHT; i++)
	{
		// Static cast seems to be required to fix a possible issue with overflow when int multiplication is cast to double
		actorList.push_back(new YellowBorderLine(false, i * static_cast<double>(SPRITE_HEIGHT), this));
		actorList.push_back(new YellowBorderLine(true, i * static_cast<double>(SPRITE_HEIGHT), this));
	}

	// Create the two white lines every 4 units
	for (int i = 0; i < (VIEW_HEIGHT / SPRITE_HEIGHT) / 4; i++)
	{
		actorList.push_back(new WhiteBorderLine(false, i * 4.0 * static_cast<double>(SPRITE_HEIGHT), this));
		auto* const white_border_line = new WhiteBorderLine(true, i * 4.0 * static_cast<double>(SPRITE_HEIGHT), this);
		actorList.push_back(white_border_line);
		unitsSinceLastAddedWhiteLine = (static_cast<double>(VIEW_HEIGHT) - SPRITE_HEIGHT) - white_border_line->getY();
	}
}

int StudentWorld::init()
{
	// Clear out the current stuff
	m_ghost_racer = nullptr;
	actorList.clear();
	m_short_circuit_end          = false;
	unitsSinceLastAddedWhiteLine = 0;

	// Setup stuff for the level stats
	m_souls_2_save = getLevel() * 2 + 5;
	m_bonus_pts    = 5000;

	// Create the two yellow border lines on the left and the right
	initialize_lines();

	// Create ghost rider
	auto* ghost_racer = new GhostRacer(this);
	actorList.push_back(ghost_racer);
	m_ghost_racer = ghost_racer;

	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::random_x_value() { return randInt(ROAD_CENTER - ROAD_WIDTH / 2.0, ROAD_CENTER + ROAD_WIDTH / 2.0); }

void StudentWorld::add_new_lines()
{
	const int newBorderY = VIEW_HEIGHT - SPRITE_HEIGHT;

	if (unitsSinceLastAddedWhiteLine >= SPRITE_HEIGHT)
	{
		actorList.push_back(new YellowBorderLine(false, newBorderY, this));
		actorList.push_back(new YellowBorderLine(true, newBorderY, this));
	}
	if (unitsSinceLastAddedWhiteLine >= 4.0 * SPRITE_HEIGHT)
	{
		actorList.push_back(new WhiteBorderLine(false, newBorderY, this));
		actorList.push_back(new WhiteBorderLine(true, newBorderY, this));
		unitsSinceLastAddedWhiteLine = 0;
	}

	// Compute the change in unitsSinceLastAddedWhiteLine in `updateUnitsSinceLastAddedWhiteLine`, called after the doSomethings
}

int StudentWorld::move()
{
	// Do the doSomethings
	{
		for (auto actorIterator = actorList.begin(); actorIterator < actorList.end(); ++actorIterator)
		{
			(*actorIterator)->doSomething();
			if (checkShortCircuitEnd())
			{
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
		}
	}
	updateUnitsSinceLastAddedWhiteLine();

	// Remove dead actors
	{
		for (auto actorIterator = actorList.begin(); actorIterator < actorList.end(); ++actorIterator)
		{
			if (!ghost_racer()->alive())
			{
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
			if (!(*actorIterator)->alive())
			{
				delete (*actorIterator);
				actorIterator = actorList.erase(actorIterator);
			}
		}
	}

	// Add new actors
	{
		add_zombie_peds();
		add_human_peds();
		add_oil_slick();
		add_new_lines();
		add_holy_water();
		add_lost_soul();
		add_zombie_cab();
	}

	// Deal with points and stuff
	{
		m_bonus_pts--;
		updateDisplayText();
	}


	// check for level completion
	{
		if (m_souls_2_save <= 0) { return GWSTATUS_FINISHED_LEVEL; }
	}

	return GWSTATUS_CONTINUE_GAME;
}

/// <summary>
/// Called by framework on loss of life or level complete
/// </summary>
void StudentWorld::cleanUp()
{
	for (auto actorIterator = actorList.begin(); actorIterator < actorList.end(); ++actorIterator)
	{
		delete *actorIterator;
	}
}


#pragma region AddNewActorHelpers
void StudentWorld::add_oil_slick()
{
	if (randInt(0, max(150 - (getLevel() * 10), 40)) == 0)
	{
		actorList.push_back(new OilSlick(random_x_value(), VIEW_HEIGHT, this));
	}
}

void StudentWorld::add_zombie_cab()
{
	// Cancel if the odds check fails
	if (randInt(0, max(100 - getLevel() * 20, 10)) != 0) { return; }

	// Randomly choose a lane selection order
	int       lanes[3];
	int       laneChoice  = -1;
	int       startHeight = -1;
	int       startSpeed  = -1;
	const int choice      = randInt(0, 2);
	switch (choice)
	{
	default:
		cerr << "Something went wrong with zombie cab lane choices" << endl;
	case 0:
		lanes[0] = 0;
		lanes[1] = 1;
		lanes[2] = 2;
		break;
	case 1:
		lanes[0] = 1;
		lanes[1] = 2;
		lanes[2] = 0;
		break;
	case 2:
		lanes[0] = 2;
		lanes[1] = 0;
		lanes[2] = 1;
		break;
	}

	// Choose where to put it inside the lane
	for (int i = 0; i < 3; i++)
	{
		// Check the bottom of the lane first
		const int closest_actor_to_bottom_dist = StudentWorld::collisionActorInLane(lanes[i], 0.0, false);
		if (closest_actor_to_bottom_dist == -1 || closest_actor_to_bottom_dist > VIEW_HEIGHT / 3)
		{
			laneChoice  = lanes[i];
			startHeight = SPRITE_HEIGHT / 2;
			startSpeed  = ghost_racer()->racer_speed() + randInt(2, 4);
			break;
		}

		// Now check the top of the lane
		const int closest_actor_to_top_dist = StudentWorld::collisionActorInLane(lanes[i], VIEW_HEIGHT, true);
		if (closest_actor_to_top_dist == -1 || (VIEW_HEIGHT - closest_actor_to_top_dist) < (VIEW_HEIGHT * 2 / 3))
		{
			laneChoice  = lanes[i];
			startHeight = VIEW_HEIGHT - SPRITE_HEIGHT / 2;
			startSpeed  = ghost_racer()->racer_speed() - randInt(2, 4);
			break;
		}
	}

	// All 3 lanes are too dangerous to add a cab, so we don't do anything
	if (laneChoice == -1) { return; }

	actorList.push_back(new ZombieCab(laneToCoord(laneChoice), startHeight, startSpeed, this));
}


void StudentWorld::add_holy_water()
{
	const int ChanceOfHolyWater = 100 + 10 * getLevel();
	if (randInt(0, ChanceOfHolyWater) == 0)
	{
		actorList.push_back(new HolyWaterGoodie(random_x_value(), VIEW_HEIGHT, this));
	}
}

void StudentWorld::add_human_peds()
{
	const int chance_of_human_ped = max(200 - getLevel() * 10, 30);
	if (randInt(0, chance_of_human_ped) == 0)
	{
		actorList.push_back(new HumanPedestrian(randInt(0, VIEW_WIDTH), VIEW_HEIGHT, this));
	}
}

void StudentWorld::add_zombie_peds()
{
	if (randInt(0, max(100 - (getLevel() * 10), 20)) == 0)
	{
		actorList.push_back(new ZombiePedestrian(randInt(0, VIEW_WIDTH), VIEW_HEIGHT, this));
	}
}

void StudentWorld::add_lost_soul()
{
	if (randInt(0, 100) == 0) { actorList.push_back(new SoulGoodie(random_x_value(), VIEW_HEIGHT, this)); }
}

void StudentWorld::addHealthPack(double startX, double startY)
{
	// 1 in 5 odds to spawn
	if (randInt(0, 4) == 0) { actorList.push_back(new HealingGoodie(startX, startY, this)); }
}

void StudentWorld::add_oil_slick(double startX, double startY)
{
	// 1 in 5 odds to spawn
	if (randInt(0, 4) == 0) { actorList.push_back(new OilSlick(startX, startY, this)); }
}

#pragma endregion

/// <summary>
/// Returns the distance to the closest collision actor in a given lane, given the described parameters.
/// Default configuration will tell the dist to the first collideable actor in a lane from the bottom of the screen going up.
/// </summary>
/// <param name="lane">The lane to check for, 0-2</param>
/// <param name="y_coord">The y-coord to search from, defaults to bottom of screen</param>
/// <param name="behind">Weather to search in front of or behind the given coord, defaults to no</param>
/// <returns>The distance to the closest collision actor in a certain lane, or -1 if none are found, or -2 if data is entered in error.</returns>
double StudentWorld::collisionActorInLane(int lane, double y_coord = 0.0, bool behind = false)
{
	double left_search_area;
	double right_search_area;
	switch (lane)
	{
	case 0:
		left_search_area = ROAD_CENTER - ROAD_WIDTH / 2.0;
		right_search_area = (ROAD_CENTER - ROAD_WIDTH / 2.0) + (ROAD_WIDTH / 3.0);
		break;
	case 1:
		left_search_area = (ROAD_CENTER - ROAD_WIDTH / 2.0) + (ROAD_WIDTH / 3.0);
		right_search_area = (ROAD_CENTER + ROAD_WIDTH / 2.0) - (ROAD_WIDTH / 3.0);
		break;
	case 2:
		left_search_area = (ROAD_CENTER + ROAD_WIDTH / 2.0) - (ROAD_WIDTH / 3.0);
		right_search_area = ROAD_CENTER + ROAD_WIDTH / 2.0;
		break;
	default:
		// Error!
		return -2;
	}

	for (auto actorIterator = actorList.begin(); actorIterator < actorList.end(); ++actorIterator)
	{
		if ((*actorIterator)->collisionAvoidanceWorthy() && (*actorIterator)->getX() >= left_search_area && (*
			actorIterator)->getX() < right_search_area)
		{
			// Collideable actor is in our lane!

			// actorY - y_coord, positive when in front, negative when behind.
			const double delta_y = (*actorIterator)->getY() - y_coord;
			if (behind)
			{
				if (delta_y < 0) { return abs(delta_y); }
				return -1;
			}
			else
			{
				if (delta_y > 0) { return abs(delta_y); }
				return -1;
			}
		}
	}
	return -1;
}

int StudentWorld::coordToLane(double x_coord)
{
	// Lane 0 (far left lane)
	if (x_coord >= ROAD_CENTER - ROAD_WIDTH / 2.0 && x_coord < (ROAD_CENTER - ROAD_WIDTH / 2.0) + (ROAD_WIDTH / 3.0))
	{
		return 0;
	}
	else if (x_coord >= (ROAD_CENTER - ROAD_WIDTH / 2.0) + (ROAD_WIDTH / 3.0) && x_coord < (ROAD_CENTER + ROAD_WIDTH /
		2.0) - (ROAD_WIDTH / 3.0)) { return 1; }
	else if (x_coord >= (ROAD_CENTER + ROAD_WIDTH / 2.0) - (ROAD_WIDTH / 3.0) && x_coord < ROAD_CENTER + ROAD_WIDTH /
		2.0) { return 2; }
	return -1;
}

double StudentWorld::laneToCoord(int lane)
{
	switch (lane)
	{
	case 0:
		return ROAD_CENTER - ROAD_WIDTH / 3.0;
	case 1: default:
		return ROAD_CENTER;
	case 2:
		return ROAD_CENTER + ROAD_WIDTH / 3.0;
	}
	
}

void StudentWorld::updateUnitsSinceLastAddedWhiteLine()
{
	// -4 is the base speed of the lines
	unitsSinceLastAddedWhiteLine += abs(-4 - ghost_racer()->racer_speed());
}

void StudentWorld::updateDisplayText()
{
	// Score: 2100 Lvl: 1 Souls2Save: 5 Lives: 3 Health: 95 Sprays: 22 Bonus: 4321
	ostringstream stream;
	stream << "Score: " << getScore() << "  Lvl: " << getLevel() << "  Souls2Save: " << m_souls_2_save << " Lives: " <<
		getLives() << " Health: " << this->ghost_racer()->get_health() << "  Sprays: " << this->ghost_racer()->
		holy_water() << "  Bonus: " << m_bonus_pts << endl;
	setGameStatText(stream.str());
}
