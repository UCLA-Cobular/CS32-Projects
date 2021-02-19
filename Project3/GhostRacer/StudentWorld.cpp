#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath) { return new StudentWorld(assetPath); }

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
	: GameWorld(assetPath), m_ghost_racer(nullptr), unitsSinceLastAddedWhiteLine(0) {}

StudentWorld::~StudentWorld() { cleanUp(); }

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
	// Clean things up from last game, if needed.
	m_ghost_racer                = nullptr;
	unitsSinceLastAddedWhiteLine = 0;
	actorList.clear();

	// Create the two yellow border lines on the left and the right
	initialize_lines();

	// Create ghost rider
	auto* ghost_racer = new GhostRacer(this);
	actorList.push_back(ghost_racer);
	m_ghost_racer = ghost_racer;

	return GWSTATUS_CONTINUE_GAME;
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


	// Add new actors
	{
		add_new_lines();
	}

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
		for (auto actorIterator = actorList.begin(); actorIterator < actorList.end(); ++actorIterator)
		{
			if (!(*actorIterator)->alive())
			{
				delete (*actorIterator);
				actorIterator = actorList.erase(actorIterator);
			}
		}
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
		delete*actorIterator;
	}
}

void StudentWorld::updateUnitsSinceLastAddedWhiteLine()
{
	// -4 is the base speed of the lines
	unitsSinceLastAddedWhiteLine += abs(-4 - ghost_racer()->racer_speed());
}
