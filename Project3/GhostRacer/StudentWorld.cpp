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
	: GameWorld(assetPath), unitsSinceLastAddedWhiteLine(0), m_ghost_racer(nullptr), m_souls_2_save(0), m_bonus_pts(5000) {}

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
	unitsSinceLastAddedWhiteLine = 0;

	// Create the two yellow border lines on the left and the right
	initialize_lines();

	// Create ghost rider
	auto* ghost_racer = new GhostRacer(this);
	actorList.push_back(ghost_racer);
	m_ghost_racer = ghost_racer;

	// Setup stuff for the level stats
	m_souls_2_save = getLevel() * 2 + 5;
	m_bonus_pts    = 5000;

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::add_holy_water()
{
	const int ChanceOfHolyWater = 100 + 10 * getLevel();
	if (randInt(0, ChanceOfHolyWater) == 0)
	{
		actorList.push_back(new HolyWaterGoodie(randInt(ROAD_CENTER - ROAD_WIDTH / 2.0, ROAD_CENTER + ROAD_WIDTH / 2.0), VIEW_HEIGHT, this));
	}
}

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
	// Add new actors
	// Loop over every actor, and doSomething
	// Remove dead actors


	// Do the doSomethings
	{
		for (auto actorIterator = actorList.begin(); actorIterator < actorList.end(); ++actorIterator)
		{
			(*actorIterator)->doSomething();
		}
	}
	updateUnitsSinceLastAddedWhiteLine();

	// Remove dead actors
	{
		// TODO: What happens when the player dies?
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
		add_new_lines();
		add_holy_water();
	}

	// Deal with points and stuff
	m_bonus_pts--;
	updateDisplayText();


	// check for level completion
	if (m_souls_2_save <= 0) { return GWSTATUS_FINISHED_LEVEL; }

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
