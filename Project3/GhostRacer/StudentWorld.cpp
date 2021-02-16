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
	: GameWorld(assetPath), unitsSinceLastAddedWhiteLine(0), m_ghost_racer(nullptr), m_souls_2_save(0), m_bonus_pts(5000), m_short_circuit_end(false) {}

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
	m_ghost_racer      = nullptr;
	actorList.clear();
	m_short_circuit_end = false;
	unitsSinceLastAddedWhiteLine = 0;

	// Setup stuff for the level stats
	m_souls_2_save      = getLevel() * 2 + 5;
	m_bonus_pts         = 5000;
	
	// Create the two yellow border lines on the left and the right
	initialize_lines();

	// Create ghost rider
	auto* ghost_racer = new GhostRacer(this);
	actorList.push_back(ghost_racer);
	m_ghost_racer = ghost_racer;

	return GWSTATUS_CONTINUE_GAME;
}

int  StudentWorld::random_x_value() { return randInt(ROAD_CENTER - ROAD_WIDTH / 2.0, ROAD_CENTER + ROAD_WIDTH / 2.0); }

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
				cerr << actorList.size() << endl;
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
	if (randInt(0, max(100-(getLevel() * 10), 20)) == 0)
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
	if (randInt(0,4) == 0)
	{
		actorList.push_back(new HealingGoodie(startX, startY, this));
	}
}
#pragma endregion

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
